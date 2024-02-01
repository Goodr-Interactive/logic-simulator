/**
 * @file
 * @brief And Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_AND_H
#define DIGISIM_ELEMENTS_AND_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_and_t {
    DiElement element;

    size_t bits;

    DiTerminal input_a;
    DiTerminal input_b;

    DiTerminal output;
} DiAnd;

/**
 * Initialize a DiAnd struct.
 *
 * @param self Pointer to initialize.
 * @param bits Bit-size for the input/output terminals.
 */
void di_and_init(DiAnd *self, size_t bits);
void di_and_destroy(DiAnd *self);

#endif // DIGISIM_ELEMENTS_AND_H
