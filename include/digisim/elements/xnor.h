/**
 * @file
 * @brief Xnor Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_XNOR_H
#define DIGISIM_ELEMENTS_XNOR_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Xnor gate element.
 *
 * To initialize/destroy, use di_xnor_init/di_xnor_destroy.
 */
typedef struct di_xnor_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Number of bits for all input and output terminals
     */
    size_t bits;

    /**
     * First gate input
     */
    DiTerminal input_a;

    /**
     * Second gate input
     */
    DiTerminal input_b;

    /**
     * Gate output (emits input_a ^ input_b)
     */
    DiTerminal output;
} DiXnor;

/**
 * Initialize a DiXnor struct.
 *
 * @memberof DiXnor
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 */
void di_xnor_init(DiXnor *self, size_t bits);

/**
 * Destroy a DiXnor struct.
 *
 * @memberof DiXnor
 * @param self Pointer to destroy
 */
void di_xnor_destroy(DiXnor *self);

#endif // DIGISIM_ELEMENTS_XNOR_H
