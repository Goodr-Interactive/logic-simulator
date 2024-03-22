/**
 * @file
 * @brief And Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_AND_H
#define DIGISIM_ELEMENTS_AND_H

#include <digisim/element.h>
#include <digisim/terminal.h>
#include <digisim/utility/gate_inputs.h>

/**
 * And gate element.
 *
 * To initialize/destroy, use di_and_init/di_and_destroy.
 */
typedef struct di_and_t {
    /**
     * Element descriptor
     */
    DiElement element;

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
} DiAnd;

/**
 * Initialize a DiAnd struct.
 *
 * @memberof DiAnd
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 * @param input_count The number of inputs for the gate. Must be greater than 2.
 */
void di_and_init(DiAnd *self, size_t bits, size_t input_count);

/**
 * Destroy a DiAnd struct.
 *
 * @memberof DiAnd
 * @param self Pointer to destroy
 */
void di_and_destroy(DiAnd *self);

#endif // DIGISIM_ELEMENTS_AND_H
