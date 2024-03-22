/**
 * @file
 * @brief Or Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_OR_H
#define DIGISIM_ELEMENTS_OR_H

#include <digisim/element.h>
#include <digisim/terminal.h>
#include <digisim/utility/gate_inputs.h>

/**
 * Or gate element.
 *
 * To initialize/destroy, use di_or_init/di_or_destroy.
 */
typedef struct di_or_t {
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
} DiOr;

/**
 * Initialize a DiOr struct.
 *
 * @memberof DiOr
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 * @param input_count The number of inputs for the gate. Must be greater than 2.
 */
void di_or_init(DiOr *self, size_t bits, size_t input_count);

/**
 * Destroy a DiOr struct.
 *
 * @memberof DiOr
 * @param self Pointer to destroy
 */
void di_or_destroy(DiOr *self);

#endif // DIGISIM_ELEMENTS_OR_H
