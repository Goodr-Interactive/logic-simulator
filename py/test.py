from logisim.project import LogisimProject
from assemble import AssembledCircuit
from digisim import Simulation, BIT_HIGH, BIT_LOW


def main():
    project = LogisimProject.parse('/Users/desgroup/Downloads/lab2_part1.circ')
    circuit = project.circuits['mux4to1']

    assembly, io = AssembledCircuit.assemble(circuit, project.circuits)

    u = io.input_named('U')
    v = io.input_named('V')
    w = io.input_named('W')
    x = io.input_named('X')
    s0 = io.input_named('S0')
    s1 = io.input_named('S1')
    m = io.output_named('M')

    simulation = Simulation()

    u.set(0, BIT_HIGH)
    s0.set(0, BIT_HIGH)

    u.emit(simulation)
    v.emit(simulation)
    w.emit(simulation)
    x.emit(simulation)
    s0.emit(simulation)
    s1.emit(simulation)

    simulation.simulate(1000)

    value = m.get(0)

    print(value)

    assert len(assembly.unconnected) == 0

    print(assembly, io)


if __name__ == '__main__':
    main()