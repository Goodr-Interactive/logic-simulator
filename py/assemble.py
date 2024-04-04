from __future__ import annotations

from logisim.pinouts import Pinout, PinIdentifier, splitter_bit_per_pin
from logisim.project import LogisimCircuit, LogisimWire, LogisimComponent

from typing import Optional
from dataclasses import dataclass

from digisim import Node, Element, LogicGate, NotGate, Input, Output, Register, Buffer, InsightElement, Terminal, Splitter, BitExtender, ConstantValue, Simulation
from digisim import GATE_AND, GATE_OR, GATE_XOR, GATE_XOR_ANY, GATE_NAND, GATE_NOR, GATE_XNOR, GATE_XNOR_ANY
from digisim import EXTENDER_POLICY_ONE, EXTENDER_POLICY_SIGN, EXTENDER_POLICY_ZERO


def create_logic_gate_element(attributes: dict[str, str], op: int) -> Element:
    width = int(attributes.get('width', '1'))
    inputs = int(attributes.get('inputs', '2'))

    return LogicGate(op, width, inputs)


def create_not_element(attributes: dict[str, str]) -> Element:
    width = int(attributes.get('width', '1'))

    return NotGate(width)


def create_pin_element(attributes: dict[str, str]) -> Element:
    is_output = bool(attributes['output']) if 'output' in attributes else False
    width = int(attributes.get('width', '1'))

    if is_output:
        return Output(width)
    else:
        return Input(width)


def create_register_element(attributes: dict[str, str]) -> Element:
    width = int(attributes.get('width', '8'))

    return Register(width)


def create_splitter_element(attributes: dict[str, str]) -> Element:
    width = int(attributes.get('incoming', '2'))
    fanout = int(attributes.get('fanout', '2'))

    splits = splitter_bit_per_pin(fanout, attributes)

    assert sum(splits) == width

    return Splitter(width, fanout, splits)


def create_bit_extender_element(attributes: dict[str, str]) -> Element:
    in_width = int(attributes.get('in_width', '8'))
    out_width = int(attributes.get('out_width', '16'))

    kind = attributes.get('type', 'sign')

    if kind == 'one':
        policy = EXTENDER_POLICY_ONE
    elif kind == 'zero':
        policy = EXTENDER_POLICY_ZERO
    elif kind == 'sign':
        policy = EXTENDER_POLICY_SIGN
    else:
        print(f"Unknown extender type {kind}.")

        policy = EXTENDER_POLICY_SIGN

    return BitExtender(policy, in_width, out_width)
def create_constant_element(attributes: dict[str, str]) -> Element:
    width = int(attributes.get('width', '1'))
    value = int(attributes.get('value', '0x1'), 0)

    return ConstantValue(width, value)


# Input/Output Pins have a special role
@dataclass
class AssembledPin:
    label: Optional[str]  # Name

    position: tuple[int, int]  # Position of Input/Output
    node: Optional[Node]  # Connected Wire

    bits: int  # Pin Width

    pass


def create_element(component: LogisimComponent) -> Optional[Element]:
    element_map = {
        'AND Gate': lambda attr: create_logic_gate_element(attr, GATE_AND),
        'OR Gate': lambda attr: create_logic_gate_element(attr, GATE_OR),
        'XOR Gate': lambda attr: create_logic_gate_element(
            attr, GATE_XOR if attr.get('xor') == 'odd' else GATE_XOR_ANY),
        'NAND Gate': lambda attr: create_logic_gate_element(attr, GATE_NAND),
        'NOR Gate': lambda attr: create_logic_gate_element(attr, GATE_NOR),
        'XNOR Gate': lambda attr: create_logic_gate_element(
            attr, GATE_XNOR if attr.get('xor') == 'odd' else GATE_XNOR_ANY),
        'NOT Gate': create_not_element,
        'Register': create_register_element,
        'Splitter': create_splitter_element,
        'Bit Extender': create_bit_extender_element,
        'Constant': create_constant_element,
    }

    element_factory = element_map.get(component.component)

    if element_factory:
        return element_factory(component.attributes)
    else:
        print(f'Missing element for {component.component}')

        return None


class EmbeddedElement(Element):
    inputs: list[Buffer]
    outputs: list[Buffer]

    pinout: Pinout

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == 'input':
            return self.inputs[index].terminal('input', None)

        if name == 'output':
            return self.outputs[index].terminal('output', None)

        raise ValueError

    def __init__(self, position: tuple[int, int], inputs: list[Buffer], outputs: list[Buffer]):
        self.inputs = inputs
        self.outputs = outputs

        self.pinout = {}

        x, y = position

        for i, output in enumerate(outputs):
            current_y = y + i * 20

            self.pinout[(x, current_y)] = PinIdentifier(
                name='output',
                index=i,
                bits=output.bits()
            )

        current_x = x - 220

        for i, value in enumerate(inputs):
            current_y = y + i * 20

            self.pinout[(current_x, current_y)] = PinIdentifier(
                name='input',
                index=i,
                bits=value.bits()
            )


@dataclass
class AssembledWire:
    wire: LogisimWire
    node: Optional[Node]


@dataclass
class AssembledElement:
    pinout: Pinout
    component: Optional[LogisimComponent]
    element: Element

    # In sub-circuits, I no longer want to include the pinout,
    # since the pin locations aren't the same in the parent circuit!
    def strip_pinout(self) -> AssembledElement:
        return AssembledElement(
            pinout={},
            component=self.component,
            element=self.element
        )


@dataclass
class AssembledIO:
    inputs: list[tuple[Optional[str], Input]]
    outputs: list[tuple[Optional[str], Output]]

    def input_named(self, name: str) -> Optional[Input]:
        for input_name, value in self.inputs:
            if input_name == name:
                return value

        return None

    def output_named(self, name: str) -> Optional[Output]:
        for output_name, value in self.outputs:
            if output_name == name:
                return value

        return None


class AssembledCircuit:
    nodes: list[AssembledWire]
    elements: list[AssembledElement]

    unconnected: dict[tuple[int, int], str]

    assembled_inputs: list[AssembledPin]
    assembled_outputs: list[AssembledPin]

    by_label: dict[str, Element]

    def insights(self) -> list[tuple[Optional[str], InsightElement]]:
        result = []

        for element in self.elements:
            if isinstance(element.element, InsightElement) and element.component:
                result.append((element.component.attributes.get('label'), element.element))

        return result

    def shake(self, simulation: Simulation):
        for element in self.elements:
            element.element.change(simulation)

    def burn_inputs(self) -> AssembledIO:
        inputs = []
        outputs = []

        for pin in self.assembled_inputs:
            value = Input(pin.bits)

            if pin.node:
                pin.node.connect(value.terminal("output", None))

            inputs.append((pin.label, value))

            self.by_label[pin.label] = value

            self.elements.append(AssembledElement(
                pinout={},
                component=None,
                element=value
            ))

        for pin in self.assembled_outputs:
            output = Output(pin.bits)

            if pin.node:
                pin.node.connect(output.terminal("input", None))

            outputs.append((pin.label, output))

            self.by_label[pin.label] = output

            self.elements.append(AssembledElement(
                pinout={},
                component=None,
                element=output
            ))

        return AssembledIO(
            inputs=inputs,
            outputs=outputs
        )

    # Returns Inputs (into this circuit), Outputs (out of this circuit)
    def burn_buffers(self) -> tuple[list[Buffer], list[Buffer]]:
        inputs = []
        outputs = []

        for pin in self.assembled_inputs:
            buffer = Buffer(pin.bits)

            terminal = buffer.terminal("output", None)

            if pin.node:
                pin.node.connect(terminal)

            inputs.append(buffer)

            self.elements.append(AssembledElement(
                pinout={},
                component=None,
                element=buffer
            ))

        for i, pin in enumerate(self.assembled_outputs):
            buffer = Buffer(pin.bits)

            terminal = buffer.terminal("input", None)

            if pin.node:
                pin.node.connect(terminal)

            outputs.append(buffer)

            self.elements.append(AssembledElement(
                pinout={},
                component=None,
                element=buffer
            ))

        return inputs, outputs

    @staticmethod
    def assemble(circuit: LogisimCircuit, project: dict[str, LogisimCircuit]) -> tuple[AssembledCircuit, AssembledIO]:
        assembled = AssembledCircuit(circuit, project, set())

        io = assembled.burn_inputs()

        return assembled, io

    def __init__(self, circuit: LogisimCircuit, project: dict[str, LogisimCircuit], stack: set[str]):
        self.nodes = []
        self.elements = []
        self.unconnected = {}

        self.assembled_inputs = []
        self.assembled_outputs = []

        self.by_label = {}

        for component in circuit.components:
            label = component.attributes.get('label')

            if component.library is None:
                # Build Custom Component

                name = component.component
                sub_circuit = project[name]

                if name in stack:
                    raise ValueError(f'Circuit {name} is in an infinite recursive chain')

                assembled = AssembledCircuit(sub_circuit, project, stack.union({name}))
                inputs, outputs = assembled.burn_buffers()

                assert len(assembled.unconnected) == 0  # Make sure any unconnected pins are propagated upwards.

                self.nodes += assembled.nodes
                self.elements += [element.strip_pinout() for element in assembled.elements]

                element = EmbeddedElement(component.position, inputs, outputs)

                self.elements.append(AssembledElement(
                    pinout=element.pinout,
                    component=None,
                    element=element
                ))
            elif component.component == 'Pin':
                # Add to Input/Output Interface

                self.unconnected[component.position] = component.component

                pin = AssembledPin(
                    label=label,
                    position=component.position,
                    bits=int(component.attributes.get('width', '1')),
                    node=None
                )

                if bool(component.attributes.get('output')):
                    self.assembled_outputs.append(pin)
                else:
                    self.assembled_inputs.append(pin)
            else:
                # Regular Element
                for value in component.pinout.keys():
                    self.unconnected[value] = component.component

                element = create_element(component)

                if element:
                    self.elements.append(AssembledElement(
                        pinout=component.pinout,
                        component=component,
                        element=element
                    ))

                    if label:
                        self.by_label[label] = element

        all_pins = self.assembled_inputs + self.assembled_outputs

        for wire in circuit.wires:
            node: Optional[Node] = None

            for endpoint in wire.endpoints:
                if endpoint in self.unconnected:
                    del self.unconnected[endpoint]

                for pin in all_pins:
                    if pin.position == endpoint:
                        if node is None:
                            node = Node(pin.bits)

                        pin.node = node

                for element in self.elements:
                    pin = element.pinout.get(endpoint)

                    if pin:
                        terminal = element.element.terminal(pin.name, pin.index)

                        if node is None:
                            node = Node(pin.bits)

                        assert terminal.bits() == node.bits()

                        node.connect(terminal)

            self.nodes.append(AssembledWire(wire=wire, node=node))

    def __del__(self):
        self.nodes = []
        self.assembled_inputs = []
        self.assembled_outputs = []

        self.elements = []
        self.by_label = {}
