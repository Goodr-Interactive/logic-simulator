/**
 * @file
 * @brief Xor Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_XOR_H
#define DIGISIM_ELEMENTS_XOR_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Xor gate element.
 *
 * To initialize/destroy, use di_xor_init/di_xor_destroy.
 */
typedef struct di_xor_t {
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
} DiXor;

/**
 * Initialize a DiXor struct.
 *
 * @memberof DiXor
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 */
void di_xor_init(DiXor *self, size_t bits);

/**
 * Destroy a DiXor struct.
 *
 * @memberof DiXor
 * @param self Pointer to destroy
 */
void di_xor_destroy(DiXor *self);

#endif // DIGISIM_ELEMENTS_XOR_H
