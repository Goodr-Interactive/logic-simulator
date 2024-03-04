/**
 * @file
 * @brief Set/Reset Latch Element
 */

#ifndef DIGISIM_ELEMENTS_SR_LATCH_H
#define DIGISIM_ELEMENTS_SR_LATCH_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * SR Latch, for holding a data.
 *
 * To initialize/destroy, use di_sr_latch_init/di_sr_latch_destroy.
 */
typedef struct di_sr_latch_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Number of bits stored in the latch
     */
    size_t bits;

    /**
     * The actual state in the SR latch.
     *
     * state[0] is the value for bit 0, state[1] is for bit 1, etc.
     *
     * State is always logical low or logical high.
     */
    bool *state;

    /**
     * Set terminal, writing a 1 to any spot will set the corresponding bit to 1.
     */
    DiTerminal set;

    /**
     * Reset terminal, writing a 1 to any spot will set the corresponding bit to 0.
     */
    DiTerminal reset;

    /**
     * The value currently contained in the bit.
     */
    DiTerminal value;
} DiSrLatch;

/**
 * Initialize a DiSrLatch struct.
 *
 * @memberof DiSrLatch
 * @param latch Pointer to initialize
 * @param bits Number of bits stored in latch
 */
void di_sr_latch_init(DiSrLatch *latch, size_t bits);

/**
 * Destroy a DiSrLatch struct.
 *
 * @memberof DiSrLatch
 * @param latch Pointer to destroy
 */
void di_sr_latch_destroy(DiSrLatch *latch);

#endif // DIGISIM_ELEMENTS_SR_LATCH_H
