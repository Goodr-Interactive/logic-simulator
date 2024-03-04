/**
 * @file
 * @brief Output Terminal Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_OUTPUT_H
#define DIGISIM_ELEMENTS_OUTPUT_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Input element, for reading the value of a node/wire.
 *
 * To initialize/destroy, use di_output_init/di_output_destroy.
 */
typedef struct di_output_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Number of bits for the output terminal
     */
    size_t bits;

    /**
     * Current signal that is received through `input`
     *
     * Convenience for reading this field is provided through di_output_get_bit.
     */
    DiSignal signal;

    /**
     * Input terminal
     *
     * This terminal is read to get the value for `signal`.
     */
    DiTerminal input;
} DiOutput;

/**
 * Initialize a DiOutput struct.
 *
 * @memberof DiOutput
 * @param output Pointer to initialize
 * @param bits Bit-size for the output terminal
 */
void di_output_init(DiOutput *input, size_t bits);

/**
 * Destroy a DiOutput struct.
 *
 * @memberof DiOutput
 * @param output Pointer to destroy
 */
void di_output_destroy(DiOutput *input);

/**
 * Returns the bit at `index` being put through this output.
 *
 * @memberof DiOutput
 * @param output Output element
 * @param index Index of the bit. (index < `input->bits`)
 * @return The value of the bit at this index
 */
DiBit di_output_get_bit(DiOutput *input, size_t index);

#endif // DIGISIM_ELEMENTS_OUTPUT_H
