/**
 * @file
 * @brief Multiplexer Component
 */

#ifndef DIGISIM_ELEMENTS_MULTIPLEXER_H
#define DIGISIM_ELEMENTS_MULTIPLEXER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Multiplexer component, wires an output to one of a few inputs.
 */
typedef struct di_multiplexer_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Number of bits in the `select` terminal
     */
    size_t select_bits;

    /**
     * Number of bits in the input/output terminals.
     */
    size_t data_bits;

    /**
     * Number of elements in the `inputs` array/number of inputs to select from.
     *
     * This number should be set to 2^select_bits (to the power of).
     */
    size_t input_count;

    /**
     * An array of `input_count` inputs.
     * The input that will be connected to output is selected based on the integer value of `select`.
     */
    DiTerminal *inputs;

    /**
     * The signal that selects which terminal is routed to output.
     *
     * A value of 0b101 (5) will connect `inputs[5]` to `output` logically.
     */
    DiTerminal select;

    /**
     * The output that will hold the value of one of the inputs.
     */
    DiTerminal output;
} DiMultiplexer;

/**
 * Initialize a DiMultiplexer struct.
 * There will be `2^select_bits` inputs.
 * .
 * @memberof DiMultiplexer
 * @param multiplexer Pointer to initialize
 * @param data_bits Number of bits for input/outputs
 * @param select_bits Number of bits for the select terminal
 */
void di_multiplexer_init(DiMultiplexer *multiplexer, size_t data_bits, size_t select_bits);

/**
 * Destroy a DiMultiplexer struct.
 *
 * @memberof DiMultiplexer
 * @param multiplexer Pointer to destroy
 */
void di_multiplexer_destroy(DiMultiplexer *multiplexer);

#endif // DIGISIM_ELEMENTS_MULTIPLEXER_H
