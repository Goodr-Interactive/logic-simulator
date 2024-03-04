/**
 * @file
 * @brief Demultiplexer Component
 */

#ifndef DIGISIM_ELEMENTS_DEMULTIPLEXER_H
#define DIGISIM_ELEMENTS_DEMULTIPLEXER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Demultiplexer component, wires an input to one of a few outputs.
 */
typedef struct di_demultiplexer_t {
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
     * Number of elements in the `outputs` array/number of outputs to select.
     *
     * This number should be set to 2^select_bits (to the power of).
     */
    size_t output_count;

    /**
     * The input that will hold the value of one of the outputs.
     */
    DiTerminal input;

    /**
     * The signal that selects which output terminal receives the input signal.
     *
     * A value of 0b101 (5) will connect `input` to `outputs[5]` logically.
     */
    DiTerminal select;

    /**
     * An array of `output_count` outputs.
     * The output that will receive the value of the input is selected based on the integer value of `select`.
     */
    DiTerminal *outputs;
} DiDemultiplexer;

/**
 * Initialize a DiDemultiplexer struct.
 * There will be `2^select_bits` outputs.
 *
 * @memberof DiDemultiplexer
 * @param demultiplexer Pointer to initialize
 * @param data_bits Number of bits for input/outputs
 * @param select_bits Number of bits for the select terminal
 */
void di_demultiplexer_init(DiDemultiplexer *demultiplexer, size_t data_bits, size_t select_bits);

/**
 * Destroy a DiDemultiplexer struct.
 *
 * @memberof DiDemultiplexer
 * @param demultiplexer Pointer to destroy
 */
void di_demultiplexer_destroy(DiDemultiplexer *demultiplexer);

#endif // DIGISIM_ELEMENTS_DEMULTIPLEXER_H
