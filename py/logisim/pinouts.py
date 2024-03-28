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

    size_half = size // 20 * 10
    size += height_offset  # XOR gates are a bit longer

    dir_x, dir_y = facing_to_direction(facing)
    norm_x, norm_y = facing_to_normal_direction(facing)

    pin1_x = x + dir_x * size + norm_x * size_half
    pin1_y = y + dir_y * size + norm_y * size_half
    pin2_x = x + dir_x * size - norm_x * size_half
    pin2_y = y + dir_y * size - norm_y * size_half

    return {
        (x, y): PinIdentifier(name='output', bits=width),
        (pin1_x, pin1_y): PinIdentifier(name='input_a', bits=width),
        (pin2_x, pin2_y): PinIdentifier(name='input_b', bits=width)
    }


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


def splitter_bit_per_pin(fanout: int, attributes: dict[str, str]) -> list[int]:
    result = [0] * fanout

    other = 0

    for i in range(fanout):
        bit_name = f'bit{i}'

        value = attributes.get(bit_name, i)

        if value is not None:
            value = int(value)

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

    bits = splitter_bit_per_pin(fanout, attributes)

    for i in range(fanout):
        pin_x = dir_x + norm_x * (i + 1) + x
        pin_y = dir_y + norm_y * (i + 1) + y

        result[(pin_x, pin_y)] = PinIdentifier(name='split', index=i, bits=bits[i])

    return result


def create_pinout(gate: str, position: tuple[int, int], attributes: dict[str, str]) -> Pinout:
    pinout_map = {
        'AND Gate': create_binary_gate_pinout,
        'OR Gate': create_binary_gate_pinout,
        'XOR Gate': lambda p, a: create_binary_gate_pinout(p, a, 10),
        'NOT Gate': create_unary_gate_pinout,
        'Pin': create_pin_pinout,
        'Register': create_register_pinout,
        'Splitter': create_splitter_pinout,
    }

    pinout_factory = pinout_map.get(gate)

    if pinout_factory:
        return pinout_factory(position, attributes)
    else:
        print(f"Missing pinout for gate {gate}")

        # Empty Pinout
        return {}
