/**
 * @file
 * @brief Logic Gate Element and Connections (AND, OR, XOR, etc.)
 */

#ifndef DIGISIM_ELEMENTS_AND_H
#define DIGISIM_ELEMENTS_AND_H

#include <digisim/element.h>
#include <digisim/terminal.h>
#include <digisim/utility/gate_inputs.h>

/**
 * The gate operation to perform. Used for DiGate.
 */
typedef enum di_gate_op {
    /**
     * Bitwise AND operation between inputs (a & b).
     */
    DI_GATE_OP_AND,
    /**
     * Bitwise OR operation between inputs (a | b).
     */
    DI_GATE_OP_OR,
    /**
     * Bitwise XOR operation between inputs (a ^ b).
     */
    DI_GATE_OP_XOR,
    /**
     * Bitwise NAND operation between inputs ~(a & b).
     */
    DI_GATE_OP_NAND,
    /**
     * Bitwise NOR operation between inputs ~(a | b).
     */
    DI_GATE_OP_NOR,
    /**
     * Bitwise XNOR operation between inputs ~(a ^ b).
     */
    DI_GATE_OP_XNOR,
} DiGateOp;

/**
 * Logic gate element.
 *
 * To initialize/destroy, use di_and_init/di_and_destroy.
 */
typedef struct di_gate_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * The operation of the logic gate (is this an AND/OR/XOR gate).
     */
    DiGateOp op;

    /**
     * Number of bits for all input and output terminals
     */
    size_t bits;

    /**
     * List of input terminals.
     */
    DiGateInputs inputs;

    /**
     * Gate output (emits input_a && input_b)
     */
    DiTerminal output;
} DiGate;

/**
 * Initialize a DiGate struct.
 *
 * @memberof DiGate
 * @param self Pointer to initialize
 * @param op The type of gate to use (AND/OR/XOR)
 * @param bits Bit-size for the input/output terminals
 * @param input_count The number of inputs for the gate. Must be greater than 2.
 */
void di_gate_init(DiGate *self, DiGateOp op, size_t bits, size_t input_count);

/**
 * Destroy a DiGate struct.
 *
 * @memberof DiGate
 * @param self Pointer to destroy
 */
void di_gate_destroy(DiGate *self);

#endif // DIGISIM_ELEMENTS_AND_H
