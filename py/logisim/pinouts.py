from dataclasses import dataclass
from typing import Optional


@dataclass
class PinIdentifier:
    name: str
    index: Optional[int] = None
    bits: int = 1


Pinout = dict[tuple[int, int], PinIdentifier]


def facing_to_direction(facing: str) -> tuple[int, int]:
    if facing == 'south':
        return 0, -1

    if facing == 'north':
        return 0, +1

    if facing == 'east':
        return -1, 0

    if facing == 'west':
        return +1, 0

    return 0, 0


def facing_to_normal_direction(facing: str) -> tuple[int, int]:
    if facing == 'south':
        return -1, 0

    if facing == 'north':
        return +1, 0

    if facing == 'east':
        return 0, -1

    if facing == 'west':
        return 0, +1

    return 0, 0


def create_binary_gate_pinout(position: tuple[int, int], attributes: dict[str, str], height_offset: int = 0) -> Pinout:
    x, y = position

    size = int(attributes.get('size', '50'))
    facing = attributes.get('facing', 'east')
    width = int(attributes.get('width', '1'))
    inputs = int(attributes.get('inputs', '2'))

    size_half = size // 20 * 10
    size += height_offset  # XOR gates are a bit longer

    dir_x, dir_y = facing_to_direction(facing)
    norm_x, norm_y = facing_to_normal_direction(facing)

    result = {
        (x, y): PinIdentifier(name='output', bits=width),
    }

    # Ordering of inputs for logic gates to terminal doesn't matter, so I pick an incremental order
    input_index = 0

    if inputs % 2 == 1:
        # odd, add middle pin
        pin_x = x + dir_x * size
        pin_y = y + dir_y * size

        result[(pin_x, pin_y)] = PinIdentifier(name='inputs', index=input_index, bits=width)
        input_index += 1

    if inputs <= 3:
        # add side pins
        pin1_x = x + dir_x * size + norm_x * size_half
        pin1_y = y + dir_y * size + norm_y * size_half
        pin2_x = x + dir_x * size - norm_x * size_half
        pin2_y = y + dir_y * size - norm_y * size_half

        result[(pin1_x, pin1_y)] = PinIdentifier(name='inputs', index=input_index, bits=width)
        input_index += 1

        result[(pin2_x, pin2_y)] = PinIdentifier(name='inputs', index=input_index, bits=width)
        input_index += 1
    else:
        input_half = inputs // 2

        for i in range(input_half):
            pin_x = x + dir_x * size + norm_x * 10 * (i + 1)
            pin_y = y + dir_y * size + norm_y * 10 * (i + 1)

            result[(pin_x, pin_y)] = PinIdentifier(name='inputs', index=input_index, bits=width)
            input_index += 1

        for i in range(input_half):
            pin_x = x + dir_x * size - norm_x * 10 * (i + 1)
            pin_y = y + dir_y * size - norm_y * 10 * (i + 1)

            result[(pin_x, pin_y)] = PinIdentifier(name='inputs', index=input_index, bits=width)
            input_index += 1

    return result


def create_unary_gate_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    x, y = position

    size = int(attributes['size']) if 'size' in attributes else 30
    facing = attributes.get('facing', 'east')
    width = int(attributes.get('width', '1'))

    dir_x, dir_y = facing_to_direction(facing)

    return {
        (x, y): PinIdentifier(name='output', bits=width),
        (x + dir_x * size, y + dir_y * size): PinIdentifier(name='input', bits=width)
    }


def create_pin_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    is_output = bool(attributes['output']) if 'output' in attributes else False
    width = int(attributes.get('width', '1'))

    if is_output:
        return {
            position: PinIdentifier(name='input', bits=width)
        }
    else:
        return {
            position: PinIdentifier(name='output', bits=width)
        }


def create_register_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    x, y = position

    width = int(attributes.get('width', '8'))

    return {
        (x, y + 30): PinIdentifier(name='data', bits=width),
        (x + 60, y + 30): PinIdentifier(name='value', bits=width),
        (x, y + 70): PinIdentifier(name='clock', bits=1),
    }


def splitter_bit_per_pin(fanout: int, width: int, attributes: dict[str, str]) -> list[int]:
    result = [0] * fanout

    for i in range(width):
        bit_name = f'bit{i}'

        value = int(attributes.get(bit_name, i))

        if value < fanout:
            result[value] += 1

    return result


def create_splitter_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    is_right = attributes.get('appear') == 'right'
    norm_multiplier = -1 if is_right else +1

    width = int(attributes.get('incoming', '2'))
    fanout = int(attributes.get('fanout', '2'))
    facing = attributes.get('facing', 'east')
    spacing = int(attributes.get('spacing', '1'))

    x, y = position

    result = {
        position: PinIdentifier(name='end', bits=width)
    }

    dir_x, dir_y = facing_to_direction(facing)
    dir_x, dir_y = -dir_x * 20, -dir_y * 20
    norm_x, norm_y = facing_to_normal_direction(facing)
    norm_x, norm_y = norm_x * norm_multiplier * spacing * 10, norm_y * norm_multiplier * spacing * 10

    bits = splitter_bit_per_pin(fanout, width, attributes)

    for i in range(fanout):
        pin_x = dir_x + norm_x * (i + 1) + x
        pin_y = dir_y + norm_y * (i + 1) + y

        result[(pin_x, pin_y)] = PinIdentifier(name='split', index=i, bits=bits[i])

    return result


def create_bit_extender_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    x, y = position

    in_width = int(attributes.get('in_width', '8'))
    out_width = int(attributes.get('out_width', '16'))

    return {
        (x - 40, y): PinIdentifier(name='input', bits=in_width),
        (x, y): PinIdentifier(name='output', bits=out_width),
    }


def create_constant_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    width = int(attributes.get('width', '1'))

    return {
        position: PinIdentifier(name='output', bits=width)
    }


def create_arithmetic_pinout(position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    x, y = position

    width = int(attributes.get('width', '8'))

    return {
        (x - 40, y - 10): PinIdentifier(name='in_a', bits=width),
        (x - 40, y + 10): PinIdentifier(name='in_b', bits=width),
        (x, y): PinIdentifier(name='output', bits=width),
    }


def create_pinout(gate: str, position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    pinout_map = {
        'AND Gate': create_binary_gate_pinout,
        'OR Gate': create_binary_gate_pinout,
        'XOR Gate': lambda p, a: create_binary_gate_pinout(p, a, 10),
        'NAND Gate': lambda p, a: create_binary_gate_pinout(p, a, 10),
        'NOR Gate': lambda p, a: create_binary_gate_pinout(p, a, 10),
        'XNOR Gate': lambda p, a: create_binary_gate_pinout(p, a, 20),
        'NOT Gate': create_unary_gate_pinout,
        'Pin': create_pin_pinout,
        'Register': create_register_pinout,
        'Splitter': create_splitter_pinout,
        'Bit Extender': create_bit_extender_pinout,
        'Constant': create_constant_pinout,
        'Power': create_constant_pinout,
        'Ground': create_constant_pinout,
        'Adder': create_arithmetic_pinout,
        'Subtractor': create_arithmetic_pinout,
        'Multiplier': create_arithmetic_pinout,
    }

    pinout_factory = pinout_map.get(gate)

    if pinout_factory:
        return pinout_factory(position, attributes)
    else:
        print(f"Missing pinout for gate {gate}")

        # Empty Pinout
        return {}
