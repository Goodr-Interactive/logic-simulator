from logisim.project import LogisimProject
from digisim import Simulation, Input, InsightElement, BIT_LOW, BIT_HIGH, BIT_ERROR, BIT_UNKNOWN
from assemble import AssembledCircuit
from lxml import etree
import json

from typing import Optional
from argparse import ArgumentParser, Namespace, FileType


def bit_to_bin_string(bit: int) -> str:
    if bit == BIT_LOW:
        return '0'

    if bit == BIT_HIGH:
        return '1'

    if bit == BIT_ERROR:
        return 'E'

    if bit == BIT_UNKNOWN:
        return 'U'

    return 'X'


def bin_string_to_bit(text: str) -> int:
    if text == '0':
        return BIT_LOW

    if text == '1':
        return BIT_HIGH

    if text == 'E':
        return BIT_ERROR

    if text == 'U':
        return BIT_UNKNOWN

    return BIT_UNKNOWN


def state_entry_to_bin_string(state: Optional[list[int]]) -> str:
    if state is None:
        return 'ASTABLE'
    else:
        # MSB is left most, so we do index 2 first, then 1, then 0, hence we have reversed
        return ''.join(bit_to_bin_string(v) for v in reversed(state))


def parse_bin_string_to_state_entry(text: str) -> list[int]:
    return [bin_string_to_bit(c) for c in text]


def increment_state(state: list[list[bool]]) -> bool:
    for i in reversed(range(len(state))):
        element = state[i]

        for k in range(len(element)):
            if element[k] == BIT_HIGH:
                element[k] = BIT_LOW
            else:
                element[k] = BIT_HIGH

                return False

    return True


# State: List (number of simulations) of List (number of inputs) of List (number of bits)
def state_to_col_table(header: list[str], state: list[list[Optional[list[int]]]]):
    row_strings = []

    for element in state:
        row_strings.append([state_entry_to_bin_string(value) for value in element])

    longest = [
        max(max(len(row_strings[i][col]) for i in range(len(row_strings))), len(header[col]))

        for col in range(len(header))
    ]

    top = ' | '.join(item.ljust(longest[i], ' ') for i, item in enumerate(header))
    divider = '-' * len(top)

    return '\n'.join([top, divider] + [
        ' | '.join(item.rjust(longest[i], ' ') for i, item in enumerate(row))

        for row in row_strings
    ])


def build_truth_table(args: Namespace):
    project = LogisimProject(etree.parse(args.file))

    if args.circuit is None:
        circuit = project.circuits[project.main]
    else:
        circuit = project.circuits[args.circuit]

    steps = int('10000' if args.max_depth is None else args.max_depth)

    assemble, io = AssembledCircuit.assemble(circuit, project.circuits)

    if len(assemble.unconnected) > 0:
        print(f'Unconnected pins: {assemble.unconnected}')

    name_id = 0

    header = []
    entries = []

    header_inputs = []
    header_outputs = []

    state = []

    for name, value in io.inputs:
        if name is None:
            name = f'input_{name_id}'
            name_id += 1

        header.append(name)
        header_inputs.append(name)

        state.append([BIT_LOW] * value.bits())

    for name, _ in io.outputs:
        if name is None:
            name = f'output_{name_id}'

            name_id += 1

        header.append(name)
        header_outputs.append(name)

    simulation = Simulation()

    assemble.shake(simulation)
    simulation.simulate(steps)

    while True:
        simulation.clear()

        for i in range(len(io.inputs)):
            _, value = io.inputs[i]
            element = state[i]

            for k in range(len(element)):
                value.set(k, element[k])

            value.emit(simulation)

        dead = simulation.simulate(steps)

        row: list[Optional[list[int]]] = [element.copy() for element in state]

        for _, output in io.outputs:
            if dead:
                row.append(None)
            else:
                value = [output.get(i) for i in range(output.bits())]

                row.append(value)

        entries.append(row)

        if increment_state(state):
            break

    if args.readable:
        print(state_to_col_table(header, entries))
    else:
        print(json.dumps({
            'pins': {
                'inputs': header_inputs,
                'outputs': header_outputs,
            },
            'values': [
                {
                    header[i]: state_entry_to_bin_string(value)

                    for i, value in enumerate(row)
                }

                for row in entries
            ]
        }))


def build_waveform(args: Namespace):
    project = LogisimProject.parse(args.file)

    if args.circuit is None:
        circuit = project.circuits[project.main]
    else:
        circuit = project.circuits[args.circuit]

    steps = int('10000' if args.max_depth is None else args.max_depth)

    assemble, io = AssembledCircuit.assemble(circuit, project.circuits)

    wave_file = json.load(args.waveform)

    waveform = wave_file['waveform']
    requests = wave_file.get('request', [])
    clock = wave_file.get('clock')

    if clock is not None:
        value = assemble.by_label.get(clock)

        if not value or not isinstance(value, Input) or value.bits() != 1:
            print(f'Component {clock} does not exist or is not an input that can be clocked.')

            return

        clock = value

    simulation = Simulation()

    assemble.shake(simulation)
    simulation.simulate(steps)

    results = []

    for row in waveform:
        additional_requests = []

        for key, value in row.items():
            if value == 'request':
                additional_requests.append(key)
            else:
                state = parse_bin_string_to_state_entry(value)

                value = assemble.by_label.get(key)

                # We can only mutate input elements.
                if not value or not isinstance(value, Input):
                    print(f'Component {key} does not exist or is not an input.')

                    return

                for k in range(len(state)):
                    value.set(k, state[k])

                value.emit(simulation)

        if clock is not None:
            clock.set(0, BIT_LOW)
            clock.emit(simulation)

        dead = simulation.simulate(steps)

        if clock is not None:
            clock.set(0, BIT_HIGH)
            clock.emit(simulation)

            # Overwrite dead value.
            dead = simulation.simulate(steps)

        # Concat iterators instead?
        all_requests = requests + additional_requests

        build = {}

        for request in all_requests:
            value = assemble.by_label.get(request)

            if not value or not isinstance(value, InsightElement):
                print(f'Component {request} does not exist or is not inspectable.')

                return

            if dead:
                build[request] = 'ASTABLE'

                continue

            value = value.insight('content')

            build[request] = value

        results.append(build)

    if args.readable:
        columns = []

        longest_left, longest_right = 0, 0

        for i in range(len(results)):
            in_value = waveform[i]
            row = results[i]

            left = ', '.join(f'{key} = {in_value[key]}' for key in in_value if in_value[key] != 'request')
            right = ', '.join(f'{key} = {row[key]}' for key in row)

            longest_left = max(longest_left, len(left))
            longest_right = max(longest_right, len(right))

            columns.append((left, right))

        clocked_indicator = ' (Clocked)' if clock is not None else ''

        left = f"Values Set{clocked_indicator}"
        right = "Retrieved Values"

        longest_left = max(longest_left, len(left))
        longest_right = max(longest_right, len(right))

        print(f'{left.ljust(longest_left)} | {right.rjust(longest_right)}')
        print('-' * (longest_left + longest_right + 3))
        print('\n'.join(f'{left.ljust(longest_left)} | {right.rjust(longest_right)}' for left, right in columns))
    else:
        print(json.dumps({
            'steps': results
        }))


def interaction(args: Namespace):
    project = LogisimProject.parse(args.file)

    if args.circuit is None:
        circuit = project.circuits[project.main]
    else:
        circuit = project.circuits[args.circuit]

    steps = int('10000' if args.max_depth is None else args.max_depth)

    assemble, io = AssembledCircuit.assemble(circuit, project.circuits)

    if len(assemble.unconnected) > 0:
        print(f'Unconnected pins: {assemble.unconnected}')

    header = []

    name_id = 0

    for name, value in io.inputs:
        if name is None:
            name = f'input_{name_id}'
            name_id += 1

        header.append(name)

    for name, _ in io.outputs:
        if name is None:
            name = f'output_{name_id}'

            name_id += 1

        header.append(name)

    simulation = Simulation()

    assemble.shake(simulation)
    simulation.simulate(steps)

    dead = False

    while True:
        row = []

        for name, value in io.inputs:
            row.append([value.get(i) for i in range(value.bits())])

        for _, output in io.outputs:
            if dead:
                row.append(None)
            else:
                value = [output.get(i) for i in range(output.bits())]

                row.append(value)

        row = [state_entry_to_bin_string(x) for x in row]
        header_lengths = [max(len(header[i]), len(row[i])) for i in range(len(header))]

        top = ' | '.join([header[i].ljust(header_lengths[i]) for i in range(len(header))])
        bottom = ' | '.join([row[i].ljust(header_lengths[i]) for i in range(len(header))])
        middle = '=' * len(top)

        print(top)
        print(middle)
        print(bottom)

        print("> ", end="")

        command = input()
        parts = [x.strip() for x in command.split("=")]

        if len(parts) != 2:
            print("Failed to parse command.")
            continue

        input_name = parts[0]
        new_value = parse_bin_string_to_state_entry(parts[1])

        input_value = assemble.by_label.get(input_name)

        # We can only mutate input elements.
        if not input_value or not isinstance(input_value, Input):
            print(f'Component {input_name} does not exist or is not an input.')

            continue

        if len(new_value) != input_value.bits():
            print(f'Component {input_name} is {input_value.bits()} wide but you entered {len(new_value)}.')

            continue

        for k in range(len(new_value)):
            input_value.set(k, new_value[k])

        input_value.emit(simulation)

        dead = simulation.simulate(steps)



def main():
    parser = ArgumentParser(
        prog='digisim',
        description='Digital Logic Simulation CLI'
    )

    subparsers = parser.add_subparsers(required=True, dest='command')

    table = subparsers.add_parser('table', help='Generate Truth Table')
    table.add_argument('file', type=FileType('r', encoding='UTF-8'))
    table.add_argument('--circuit')
    table.add_argument('--max-depth')
    table.add_argument('--readable', action='store_true')

    wave = subparsers.add_parser('wave', help='Generate Waveform')
    wave.add_argument('file', type=FileType('r', encoding='UTF-8'))
    wave.add_argument('--waveform', type=FileType('r', encoding='UTF-8'), required=True)
    wave.add_argument('--circuit', required=False)
    wave.add_argument('--max-depth')
    wave.add_argument('--readable', action='store_true')

    interact = subparsers.add_parser('interact', help='Live Circuit Interaction')
    interact.add_argument('file', type=FileType('r', encoding='UTF-8'))
    interact.add_argument('--circuit', required=False)
    interact.add_argument('--max-depth')

    args = parser.parse_args()

    if args.command == 'table':
        build_truth_table(args)

    if args.command == 'wave':
        build_waveform(args)

    if args.command == 'interact':
        interaction(args)


if __name__ == '__main__':
    main()
