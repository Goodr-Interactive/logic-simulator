/**
 * @file
 * @brief Buffer Element
 */

#ifndef DIGISIM_ELEMENTS_BUFFER_H
#define DIGISIM_ELEMENTS_BUFFER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Buffer element.
 *
 * To initialize/destroy, use di_buffer_init/di_buffer_destroy.
 */
typedef struct di_buffer_t {
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
     * Gate output (emits input)
     */
    DiTerminal output;
} DiBuffer;

/**
 * Initialize a DiBuffer struct.
 *
 * @memberof DiBuffer
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 */
void di_buffer_init(DiBuffer *self, size_t bits);

/**
 * Destroy a DiBuffer struct.
 *
 * @memberof DiBuffer
 * @param self Pointer to destroy
 */
void di_buffer_destroy(DiBuffer *self);

#endif // DIGISIM_ELEMENTS_BUFFER_H
