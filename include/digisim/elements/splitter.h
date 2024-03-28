/**
* @file
* @brief Bit Splitter Element
 */

#ifndef DIGISIM_ELEMENTS_SPLITTER_H
#define DIGISIM_ELEMENTS_SPLITTER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Splits an input signal `end` into many smaller terminals.
 * Each terminal will be equal to a subset of the bits in `end` (e.g. splits[0] is bits 0-4, splits[1] is bits 5-7).
 *
 * To initialize/destroy, use di_splitter_init/di_splitter_destroy.
 */
typedef struct di_splitter_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Bit width of the end terminal (sum of all the bits of the terminals in splits)
     */
    size_t bits;

    /**
     * End terminal, acts as an input/output to all the terminals in `splits`
     */
    DiTerminal end;

    /**
     * Number of terminals in the `splits` array
     */
    size_t split_count;

    /**
     * An array of terminals. The sum of all the `splits[i]->bits` should be equal to `bits` (on this struct).
     *
     * splits[0] is connected to bits 0 - (splits[0]->bits - 1) of end
     * splits[1] is connected to bits splits[0]->bits - (splits[0]->bits + splits[1]->bits - 1) of end
     * etc.
     */
    DiTerminal *splits;

    /**
     * Temporary signal for merging the value of each split.
     *
     * For internal use. Should be equal to the signal of end after a changed call.
     */
    DiSignal split_accumulator;
} DiSplitter;

/**
 * Initialize a DiSplitter struct.
 *
 * @memberof DiSplitter
 * @param splitter Pointer to initialize
 * @param bits Number of bits
 * @param split_count The number of integers in the splits parameter, and the number of split terminals in the splitter.
 * @param splits The value of parameter splits[i] will be the bit width of terminal splits[i].
 *               See splits field for details as how the splitter field works.
 */
void di_splitter_init(DiSplitter *splitter, size_t bits, size_t split_count, const size_t *splits);

/**
 * Destroy a DiSplitter struct.
 *
 * @memberof DiSplitter
 * @param splitter Pointer to destroy
 */
void di_splitter_destroy(DiSplitter *splitter);

#endif // DIGISIM_ELEMENTS_SPLITTER_H
