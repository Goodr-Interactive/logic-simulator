from cpython.mem cimport PyMem_Malloc, PyMem_Free

from typing import Optional

BIT_LOW = 0
BIT_HIGH = 1
BIT_ERROR = 2
BIT_UNKNOWN = 3

GATE_AND = 0
GATE_OR = 1
GATE_XOR = 2
GATE_XOR_ANY = 3
GATE_NAND = 4
GATE_NOR = 5
GATE_XNOR = 6
GATE_XNOR_ANY = 7

EXTENDER_POLICY_ZERO = 0
EXTENDER_POLICY_ONE = 1
EXTENDER_POLICY_SIGN = 2

ARITHMETIC_OP_ADD = 0
ARITHMETIC_OP_SUB = 1
ARITHMETIC_OP_MUL = 2
ARITHMETIC_OP_MUL_SIGNED = 3

cdef class Simulation:
    cdef DiSimulation *simulation
    cdef DiZeroSimulation *zero

    def simulate(self, steps: int) -> bool:
        return di_simulation_run(self.simulation, steps) == 1

    def clear(self):
        di_simulation_clear(self.simulation)

    def count(self) -> int:
        return self.zero.count

    def add(self):
        di_simulation_add(self.simulation, NULL)

    def __init__(self):
        self.simulation = di_simulation_create()
        self.zero = <DiZeroSimulation *>self.simulation

    def __del__(self):
        di_simulation_free(self.simulation)

cdef class Terminal:
    cdef DiTerminal *terminal
    cdef object parent

    def bits(self) -> int:
        return self.terminal.bits

    def term_id(self) -> int:
        return <size_t>self.terminal

    @staticmethod
    cdef create(object parent, DiTerminal *terminal):
        term = Terminal()

        term.terminal = terminal
        term.parent = parent

        return term

cdef class Node:
    cdef DiNode *node

    def bits(self) -> int:
        return self.node.bits

    def connect(self, terminal: Terminal):
        assert self.node.bits == terminal.terminal.bits
        assert terminal.terminal.node == NULL

        di_connect(self.node, terminal.terminal)

    def disconnect(self, terminal: Terminal):
        di_disconnect(self.node, terminal.terminal)

    def get(self) -> list[int]:
        return [di_signal_get(&self.node.signal, i) for i in range(self.node.signal.bits)]

    def __init__(self, bits: int):
        self.node = <DiNode *> PyMem_Malloc(sizeof(DiNode))

        di_node_init(self.node, bits)

    def __del__(self):
        di_node_destroy(self.node)

        PyMem_Free(self.node)


cdef class Element:
    def change(self, simulation: Simulation):
        pass

    def reset(self):
        pass

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        raise NotImplementedError

cdef class InsightElement(Element):
    def insight(self, name: str) -> str:
        raise NotImplementedError


cdef class LogicGate(Element):
    cdef DiGate *gate

    def change(self, simulation: Simulation):
        di_element_changed(&self.gate.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "inputs":
            return Terminal.create(self, di_gate_inputs_get(&self.gate.inputs, index))

        if name == "output":
            return Terminal.create(self, &self.gate.output)

    # op is GATE_AND/GATE_OR/GATE_XOR/etc.
    def __init__(self, op: int, bits: int, input_count: int):
        self.gate = <DiGate *> PyMem_Malloc(sizeof(DiGate))

        di_gate_init(self.gate, op, bits, input_count)

    def __del__(self):
        di_gate_destroy(self.gate)

        PyMem_Free(self.gate)


cdef class NotGate(Element):
    cdef DiNot *gate

    def change(self, simulation: Simulation):
        di_element_changed(&self.gate.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "input":
            return Terminal.create(self, &self.gate.input)

        if name == "output":
            return Terminal.create(self, &self.gate.output)

    def __init__(self, bits: int):
        self.gate = <DiNot *> PyMem_Malloc(sizeof(DiNot))

        di_not_init(self.gate, bits)

    def __del__(self):
        di_not_destroy(self.gate)

        PyMem_Free(self.gate)

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

cdef class Input(InsightElement):
    cdef DiInput *gate

    def change(self, simulation: Simulation):
        di_element_changed(&self.gate.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "output":
            return Terminal.create(self, &self.gate.output)

    def insight(self, name: str) -> str:
        value = [
            bit_to_bin_string(di_signal_get(&self.gate.signal, i))

            for i in range(self.gate.bits)
        ]

        return ''.join(value)

    def set(self, index: int, bit: int):
        di_signal_set(&self.gate.signal, index, bit)

    def get(self, index: int) -> int:
        return di_signal_get(&self.gate.signal, index)

    def emit(self, simulation: Simulation):
        di_input_emit(self.gate, simulation.simulation)

    def bits(self) -> int:
        return self.gate.bits

    def __init__(self, bits: int):
        self.gate = <DiInput *> PyMem_Malloc(sizeof(DiInput))

        di_input_init(self.gate, bits)

    def __del__(self):
        di_input_destroy(self.gate)

        PyMem_Free(self.gate)

cdef class Output(InsightElement):
    cdef DiOutput *gate

    def change(self, simulation: Simulation):
        di_element_changed(&self.gate.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "input":
            return Terminal.create(self, &self.gate.input)

    def insight(self, name: str) -> str:
        value = [
            bit_to_bin_string(di_signal_get(&self.gate.signal, i))

            for i in range(self.gate.bits)
        ]

        return ''.join(value)

    def get(self, index: int) -> int:
        return di_output_get_bit(self.gate, index)

    def bits(self) -> int:
        return self.gate.bits

    def __init__(self, bits: int):
        self.gate = <DiOutput *> PyMem_Malloc(sizeof(DiOutput))

        di_output_init(self.gate, bits)

    def __del__(self):
        di_output_destroy(self.gate)

        PyMem_Free(self.gate)

cdef class Register(InsightElement):
    cdef DiRegister *reg

    def change(self, simulation: Simulation):
        di_element_changed(&self.reg.element, simulation.simulation)

    def reset(self):
        di_element_reset(&self.reg.element)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "data":
            return Terminal.create(self, &self.reg.data)

        if name == "clock":
            return Terminal.create(self, &self.reg.clock)

        if name == "reset":
            return Terminal.create(self, &self.reg.reset)

        if name == "value":
            return Terminal.create(self, &self.reg.value)

    def insight(self, name: str) -> str:
        value = ['1' if self.reg.state[i] else '0' for i in range(self.reg.bits)]

        return ''.join(value)

    def __init__(self, bits: int):
        self.reg = <DiRegister *> PyMem_Malloc(sizeof(DiRegister))

        di_register_init(self.reg, bits)

    def __del__(self):
        di_register_destroy(self.reg)

        PyMem_Free(self.reg)


cdef class Buffer(Element):
    cdef DiBuffer *buffer

    def change(self, simulation: Simulation):
        di_element_changed(&self.buffer.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "input":
            return Terminal.create(self, &self.buffer.input)

        if name == "output":
            return Terminal.create(self, &self.buffer.output)

    def bits(self) -> int:
        return self.buffer.bits

    def __init__(self, bits: int):
        self.buffer = <DiBuffer *> PyMem_Malloc(sizeof(DiBuffer))

        di_buffer_init(self.buffer, bits)

    def __del__(self):
        di_buffer_destroy(self.buffer)

        PyMem_Free(self.buffer)


cdef class Connector(Element):
    cdef DiConnector *connector

    def change(self, simulation: Simulation):
        di_element_changed(&self.connector.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "input":
            return Terminal.create(self, &self.connector.connection_a)

        if name == "output":
            return Terminal.create(self, &self.connector.connection_b)

    def bits(self) -> int:
        return self.connector.bits

    def __init__(self, bits: int):
        self.connector = <DiConnector *> PyMem_Malloc(sizeof(DiConnector))

        di_connector_init(self.connector, bits)

    def __del__(self):
        di_connector_destroy(self.connector)

        PyMem_Free(self.connector)


cdef class Splitter(Element):
    cdef DiSplitter *splitter

    def change(self, simulation: Simulation):
        di_element_changed(&self.splitter.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "end":
            return Terminal.create(self, &self.splitter.end)

        if name == "split":
            assert index < self.splitter.split_count

            return Terminal.create(self, &self.splitter.splits[index])

    def __init__(self, bits: int, split_count: int, splits: list[int]):
        self.splitter = <DiSplitter *> PyMem_Malloc(sizeof(DiSplitter))

        cdef size_t *split_array = <size_t *> PyMem_Malloc(sizeof(size_t) * split_count)

        for i in range(split_count):
            split_array[i] = splits[i]

        di_splitter_init(self.splitter, bits, split_count, split_array)

        values = [self.splitter.splits[i].bits for i in range(split_count)]

    def __del__(self):
        di_splitter_destroy(self.splitter)

        PyMem_Free(self.splitter)


cdef class BitExtender(Element):
    cdef DiBitExtender *extender

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "input":
            return Terminal.create(self, &self.extender.input)

        if name == "output":
            return Terminal.create(self, &self.extender.output)

    def __init__(self, policy: int, in_bits: int, out_bits: int):
        self.extender = <DiBitExtender *> PyMem_Malloc(sizeof(DiBitExtender))

        di_bit_extender_init(self.extender, policy, in_bits, out_bits)

    def __del__(self):
        di_bit_extender_destroy(self.extender)

        PyMem_Free(self.extender)


cdef class Multiplexer(Element):
    cdef DiMultiplexer *multiplexer

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "input":
            assert index is not None and index < self.multiplexer.input_count

            return Terminal.create(self, &self.multiplexer.inputs[index])

        if name == "output":
            return Terminal.create(self, &self.multiplexer.output)

        if name == "select":
            return Terminal.create(self, &self.multiplexer.select)

    def __init__(self, data_bits: int, select_bits: int):
        self.multiplexer = <DiMultiplexer *> PyMem_Malloc(sizeof(DiMultiplexer))

        di_multiplexer_init(self.multiplexer, data_bits, select_bits)

    def __del__(self):
        di_multiplexer_destroy(self.multiplexer)

        PyMem_Free(self.multiplexer)


cdef class ConstantValue(Element):
    cdef DiConstant *constant

    def change(self, simulation: Simulation):
        di_element_changed(&self.constant.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "output":
            return Terminal.create(self, &self.constant.output)

    def __init__(self, bits: int, value: int):
        self.constant = <DiConstant *> PyMem_Malloc(sizeof(DiConstant))

        di_constant_init(self.constant, bits, value)

    def __del__(self):
        di_constant_destroy(self.constant)

        PyMem_Free(self.constant)


cdef class Arithmetic(Element):
    cdef DiArithmetic *arithmetic

    def change(self, simulation: Simulation):
        di_element_changed(&self.arithmetic.element, simulation.simulation)

    def terminal(self, name: str, index: Optional[int]) -> Terminal:
        if name == "in_a":
            return Terminal.create(self, &self.arithmetic.in_a)

        if name == "in_b":
            return Terminal.create(self, &self.arithmetic.in_b)

        if name == "output":
            return Terminal.create(self, &self.arithmetic.output)

    def __init__(self, bits: int, op: int):
        self.arithmetic = <DiArithmetic *> PyMem_Malloc(sizeof(DiArithmetic))

        di_arithmetic_init(self.arithmetic, bits, op)

    def __del__(self):
        di_arithmetic_destroy(self.arithmetic)

        PyMem_Free(self.arithmetic)
