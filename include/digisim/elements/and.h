/**
 * @file
 * @brief And Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_AND_H
#define DIGISIM_ELEMENTS_AND_H

#include <digisim/element.h>
#include <digisim/terminal.h>

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
     * First gate input
     */
    DiTerminal input_a;

    /**
     * Second gate input
     */
    DiTerminal input_b;

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
 */
void di_and_init(DiAnd *self, size_t bits);

/**
 * Destroy a DiAnd struct.
 *
 * @memberof DiAnd
 * @param self Pointer to destroy
 */
void di_and_destroy(DiAnd *self);

#endif // DIGISIM_ELEMENTS_AND_H
