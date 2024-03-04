/**
 * @file
 * @brief Nor Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_NOR_H
#define DIGISIM_ELEMENTS_NOR_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Nor gate element.
 *
 * To initialize/destroy, use di_nor_init/di_nor_destroy.
 */
typedef struct di_nor_t {
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
     * Gate output (emits ~(input_a || input_b))
     */
    DiTerminal output;
} DiNor;

/**
 * Initialize a DiNor struct.
 *
 * @memberof DiNor
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 */
void di_nor_init(DiNor *self, size_t bits);

/**
 * Destroy a DiNor struct.
 *
 * @memberof DiNor
 * @param self Pointer to destroy
 */
void di_nor_destroy(DiNor *self);

#endif // DIGISIM_ELEMENTS_NOR_H
