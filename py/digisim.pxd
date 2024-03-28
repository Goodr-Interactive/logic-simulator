from libc.stdint cimport uint64_t

cdef extern from "digisim/signal.h":
    ctypedef enum DiBit:
        DI_BIT_LOW = 0b00
        DI_BIT_UNKNOWN = 0b01
        DI_BIT_ERROR = 0b10
        DI_BIT_HIGH = 0b11

    ctypedef union DiSignalContent:
        uint64_t local
        uint64_t *heap

    ctypedef struct DiSignal:
        size_t bits

        DiSignalContent content

    DiBit di_bit_logical(bint value)
    bint di_bit_value(DiBit bit, bint default_value)
    DiBit di_signal_get(DiSignal *signal, size_t index)
    void di_signal_set(DiSignal *signal, size_t index, DiBit bit)
    void di_signal_fill(DiSignal *signal, DiBit bit)
    bint di_signal_equal(DiSignal *signal, DiSignal *other)
    void di_signal_copy(DiSignal *signal, DiSignal *source)
    uint64_t *di_signal_values(DiSignal *signal)
    void di_signal_init(DiSignal *signal, size_t bits)
    void di_signal_init_from(DiSignal *signal, DiSignal *source)
    void di_signal_destroy(DiSignal *signal)


cdef extern from "digisim/terminal.h":
    ctypedef struct DiTerminal:
        DiElement *parent
        size_t bits
        DiNode *node
        bint holding
        DiSignal signal

    void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits)
    void di_terminal_destroy(DiTerminal *terminal)
    void di_terminal_write(DiTerminal *terminal, DiSignal move_signal, DiSimulation *simulation)
    void di_terminal_reset(DiTerminal *terminal, DiSimulation *simulation)
    DiSignal *di_terminal_read(DiTerminal *terminal)


cdef extern from "digisim/utility/node-list.h":
    ctypedef union DiNodeListContent:
        DiNode *local[4]

        DiNode **heap

    ctypedef struct DiNodeList:
        size_t count
        size_t capacity
        bint heap_alloc

        DiNodeListContent content

cdef extern from "digisim/utility/terminal-list.h":
    ctypedef union DiTerminalListContent:
        DiTerminal *local[4]
        DiTerminal **heap

    ctypedef struct DiTerminalList:
        size_t count
        size_t capacity
        bint heap_alloc

        DiTerminalListContent content

cdef extern from "digisim/element.h":
    ctypedef union DiNodeListContent:
        DiNode *local[4];

        DiNode **heap;


    ctypedef struct DiNodeList:
        size_t count
        size_t capacity
        bint heap_alloc

        DiNodeListContent content

    ctypedef void (*DiChangedCallback)(DiElement *element, DiSimulation *simulation)
    ctypedef void(*DiResetCallback)(DiElement *element, DiSimulation *simulation)

    ctypedef struct DiElement:
        DiChangedCallback changed
        DiResetCallback reset

        DiTerminalList connections

    void di_element_changed(DiElement *element, DiSimulation *simulation)
    void di_element_reset(DiElement *element, DiSimulation *simulation)
    void di_element_disconnect_all(DiElement *element, DiSimulation *simulation)
    void di_element_init(DiElement *element)
    void di_element_destroy(DiElement *element)

cdef extern from "digisim/node.h":
    ctypedef union DiNodeConnectionsContent:
        DiTerminal *local[4]
        DiTerminal **heap


    ctypedef struct DiNodeConnections:
        size_t count
        size_t capacity
        bint heap_alloc

        DiNodeConnectionsContent content

    ctypedef struct DiNode:
        DiTerminalList connections
        size_t bits
        DiSignal signal
        DiSignal last_signal

    void di_node_init(DiNode *node, size_t bits)
    void di_node_destroy(DiNode *node)
    void di_node_changed(DiNode *node, DiSimulation *simulation)
    void di_node_propagate(DiNode *node, DiSimulation *simulation)
    void di_connect(DiNode *node, DiTerminal *terminal)
    void di_disconnect(DiNode *node, DiTerminal *terminal)
    void di_connect_simulate(DiNode *node, DiTerminal *terminal, DiSimulation *simulation)
    void di_disconnect_simulate(DiNode *node, DiTerminal *terminal, DiSimulation *simulation)


cdef extern from "digisim/simulation.h":
    ctypedef void (*DiSimulationAddCallback)(DiSimulation *simulation, DiNode *node)
    ctypedef void (*DiSimulationClearCallback)(DiSimulation *simulation)
    ctypedef bint (*DiSimulationRunCallback)(DiSimulation *simulation, size_t max_depth)

    ctypedef struct DiSimulation:
        DiSimulationAddCallback add
        DiSimulationClearCallback clear
        DiSimulationRunCallback run

    ctypedef struct DiNode:
        pass

    bint di_simulation_run(DiSimulation *simulation, size_t max_step)
    void di_simulation_clear(DiSimulation *simulation)
    void di_simulation_add(DiSimulation *simulation, DiNode *node)
    DiSimulation *di_simulation_create()
    void di_simulation_free(DiSimulation *simulation)


cdef extern from "digisim/simulations/zero-delay.h":
    ctypedef struct DiZeroSimulationEntry:
        DiNode *node
        size_t depth

    ctypedef struct DiZeroSimulation:
        DiSimulation simulation
        size_t count
        size_t capacity
        size_t current_depth
        DiZeroSimulationEntry *buffer

    void di_zero_simulation_init(DiZeroSimulation *simulation)
    void di_zero_simulation_destroy(DiZeroSimulation *simulation)

cdef extern from "digisim/utility/gate_inputs.h":
    ctypedef union DiGateInputsContent:
        DiTerminal local[2]
        DiTerminal *heap


    ctypedef struct DiGateInputs:
        size_t count
        DiGateInputsContent content

    DiTerminal *di_gate_inputs_get(DiGateInputs *inputs, size_t index)

    void di_gate_inputs_init(DiGateInputs *inputs, size_t count)
    void di_gate_inputs_destroy(DiGateInputs *inputs)

cdef extern from "digisim/elements/gate.h":
    ctypedef enum DiGateOp:
        DI_GATE_OP_AND = 0,
        DI_GATE_OP_OR = 1,
        DI_GATE_OP_XOR = 2,
        DI_GATE_OP_XOR_ANY = 3
        DI_GATE_OP_NAND = 4,
        DI_GATE_OP_NOR = 5,
        DI_GATE_OP_XNOR = 6,
        DI_GATE_OP_XNOR_ANY = 7,

    ctypedef struct DiGate:
        DiElement element
        DiGateOp op
        size_t bits
        DiGateInputs inputs
        DiTerminal output

    void di_gate_init(DiGate *self, DiGateOp op, size_t bits, size_t input_count)
    void di_gate_destroy(DiGate *self)

cdef extern from "digisim/elements/not.h":
    ctypedef struct DiNot:
        DiElement element
        size_t bits
        DiTerminal input
        DiTerminal output

    void di_not_init(DiNot *self, size_t bits)
    void di_not_destroy(DiNot *self)

cdef extern from "digisim/elements/input.h":
    ctypedef struct DiInput:
        DiElement element
        size_t bits
        DiSignal signal
        DiTerminal output

    void di_input_init(DiInput *input, size_t bits)
    void di_input_destroy(DiInput *input)
    void di_input_emit(DiInput *input, DiSimulation *simulation)
    DiBit di_input_get_bit(DiInput *input, size_t index)
    void di_input_set_bit(DiInput *input, size_t index, DiBit bit, DiSimulation *simulation)
    void di_input_set(DiInput *input, DiSignal move_signal, DiSimulation *simulation)

cdef extern from "digisim/elements/output.h":
    ctypedef struct DiOutput:
        DiElement element
        size_t bits
        DiSignal signal
        DiTerminal input

    void di_output_init(DiOutput *output, size_t bits)
    void di_output_destroy(DiOutput *output)
    DiBit di_output_get_bit(DiOutput *output, size_t index)

cdef extern from "digisim/elements/register.h":
    ctypedef struct DiRegister:
        DiElement element
        size_t bits
        bint did_send
        bint *hold
        bint *state
        DiTerminal data
        DiTerminal clock
        DiTerminal reset
        DiTerminal value

    void di_register_init(DiRegister *latch, size_t bits)
    void di_register_destroy(DiRegister *latch)

cdef extern from "digisim/elements/buffer.h":
    ctypedef struct DiBuffer:
        DiElement element
        size_t bits
        DiTerminal input
        DiTerminal output

    void di_buffer_init(DiBuffer *self, size_t bits)
    void di_buffer_destroy(DiBuffer *self)
