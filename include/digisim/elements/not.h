/**
 * @file
 * @brief Not Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_NOT_H
#define DIGISIM_ELEMENTS_NOT_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Not gate element.
 *
 * To initialize/destroy, use di_not_init/di_not_destroy.
 */
typedef struct di_not_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Number of bits for all input and output terminals
     */
    size_t bits;

    /**
     * Gate input
     */
    DiTerminal input;

    /**
     * Gate output (emits ~input)
     */
    DiTerminal output;
} DiNot;

/**
 * Initialize a DiNot struct.
 *
 * @memberof DiNot
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 */
void di_not_init(DiNot *self, size_t bits);

/**
 * Destroy a DiNot struct.
 *
 * @memberof DiNot
 * @param self Pointer to destroy
 */
void di_not_destroy(DiNot *self);

#endif // DIGISIM_ELEMENTS_NOT_H
