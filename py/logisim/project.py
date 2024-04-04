from __future__ import annotations

from typing import Optional, cast
from lxml import etree
from logisim.pinouts import Pinout, create_pinout


def parse_point(point: str) -> tuple[int, int]:
    return cast(tuple[int, int], tuple(int(x) for x in point[1:-1].split(',')))


class LogisimComponent:
    library: Optional[str]
    component: str

    position: tuple[int, int]
    attributes: dict[str, str]

    pinout: Pinout

    def __init__(self, root: etree.Element):
        self.library = root.get('lib')
        self.component = root.get('name')
        self.position = cast(tuple[int, int], parse_point(root.get('loc')))
        self.attributes = {a.get('name'): a.get('val') for a in root.findall('./a')}

        # None library should mean custom component (e.g. project circuit)
        # We can't possibly determine the pinout until we build the circuit
        if self.library is not None:
            self.pinout = create_pinout(self.component, self.position, self.attributes)
        else:
            self.pinout = {}


class LogisimWire:
    endpoints: set[tuple[int, int]]


class LogisimCircuit:
    name: str

    wires: list[LogisimWire]
    components: list[LogisimComponent]

    def wire_at(self, point: tuple[int, int]) -> Optional[LogisimWire]:
        for wire in self.wires:
            if point in wire.endpoints:
                return wire

        return None

    def add_wire(self, from_loc: tuple[int, int], to_loc: tuple[int, int]):
        from_wire = self.wire_at(from_loc)
        to_wire = self.wire_at(to_loc)

        if from_wire and to_wire:
            if from_wire is not to_wire:
                from_wire.endpoints = from_wire.endpoints.union(to_wire.endpoints)

                self.wires.remove(to_wire)
        elif from_wire:
            from_wire.endpoints.add(to_loc)
        elif to_wire:
            to_wire.endpoints.add(from_loc)
        else:
            wire_object = LogisimWire()
            wire_object.endpoints = {from_loc, to_loc}

            self.wires.append(wire_object)

    def __init__(self, root: etree.Element):
        self.name = root.get('name')

        components = root.findall('./comp')
        wires = root.findall('./wire')

        self.wires = []

        for wire in wires:
            from_loc = parse_point(wire.get('from'))
            to_loc = parse_point(wire.get('to'))

            self.add_wire(from_loc, to_loc)

        self.components = [LogisimComponent(comp) for comp in components]

        # Connect components at the same spot even if there is no physical wire.
        for component in self.components:
            for position in component.pinout:
                self.add_wire(position, position)


class LogisimProject:
    main: str
    libraries: dict[str, str]

    circuits: dict[str, LogisimCircuit]

    @staticmethod
    def parse(file: str) -> LogisimProject:
        return LogisimProject(etree.parse(file))

    def __init__(self, xml: etree):
        root = xml.getroot()

        self.main = root.find('./main').get('name')
        self.libraries = {lib.get('name'): lib.get('desc') for lib in root.find('./lib')}

        self.circuits = {}

        for circuit in root.findall('./circuit'):
            circuit = LogisimCircuit(circuit)

            self.circuits[circuit.name] = circuit
