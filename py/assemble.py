from logisim.project import LogisimCircuit, LogisimWire, LogisimComponent

from typing import Optional

from digisim import Node, Element, AndGate, OrGate, NotGate, Input, Output, Register, InsightElement


def create_and_element(attributes: dict[str, str]) -> Element:
    width = int(attributes.get('width', '1'))

    return AndGate(width)


def create_or_element(attributes: dict[str, str]) -> Element:
    width = int(attributes.get('width', '1'))

    return OrGate(width)


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


def create_element(component: LogisimComponent) -> Optional[Element]:
    element_map = {
        'AND Gate': create_and_element,
        'OR Gate': create_or_element,
        'NOT Gate': create_not_element,
        'Pin': create_pin_element,
        'Register': create_register_element,
    }

    element_factory = element_map.get(component.component)

    if element_factory:
        return element_factory(component.attributes)
    else:
        print(f'Missing element for {component.component}')

        return None


class AssembledCircuit:
    nodes: dict[LogisimWire, Optional[Node]]
    elements: dict[LogisimComponent, Element]

    unconnected: dict[tuple[int, int], str]

    by_label: dict[str, Element]

    def inputs(self) -> list[tuple[Optional[str], Input]]:
        result = []

        for component, element in self.elements.items():
            if isinstance(element, Input):
                result.append((component.attributes.get('label'), element))

        return result

    def outputs(self) -> list[tuple[Optional[str], Output]]:
        result = []

        for component, element in self.elements.items():
            if isinstance(element, Output):
                result.append((component.attributes.get('label'), element))

        return result

    def insights(self) -> list[tuple[Optional[str], InsightElement]]:
        result = []

        for component, element in self.elements.items():
            if isinstance(element, InsightElement):
                result.append((component.attributes.get('label'), element))

        return result

    def __init__(self, circuit: LogisimCircuit):
        self.nodes = {}
        self.elements = {}
        self.unconnected = {}

        self.by_label = {}

        for component in circuit.components:
            label = component.attributes.get('label')

            for value in component.pinout.keys():
                self.unconnected[value] = component.component

            element = create_element(component)

            if element:
                self.elements[component] = element

                if label:
                    self.by_label[label] = element

        for wire in circuit.wires:
            node: Optional[Node] = None

            for endpoint in wire.endpoints:
                if endpoint in self.unconnected:
                    del self.unconnected[endpoint]

                for component in self.elements:
                    pin = component.pinout.get(endpoint)

                    if pin:
                        terminal = self.elements[component].terminal(pin.name, pin.index)

                        if node is None:
                            node = Node(pin.bits)

                        node.connect(terminal)

            self.nodes[wire] = node
