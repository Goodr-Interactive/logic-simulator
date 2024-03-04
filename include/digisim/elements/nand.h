/**
 * @file
 * @brief Nand Gate Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_NAND_H
#define DIGISIM_ELEMENTS_NAND_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Nand gate element.
 *
 * To initialize/destroy, use di_nand_init/di_nand_destroy.
 */
typedef struct di_nand_t {
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
     * Gate output (emits ~(input_a && input_b))
     */
    DiTerminal output;
} DiNand;

/**
 * Initialize a DiNand struct.
 *
 * @memberof DiNand
 * @param self Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 */
void di_nand_init(DiNand *self, size_t bits);

/**
 * Destroy a DiNand struct.
 *
 * @memberof DiNand
 * @param self Pointer to destroy
 */
void di_nand_destroy(DiNand *self);

#endif // DIGISIM_ELEMENTS_NAND_H
