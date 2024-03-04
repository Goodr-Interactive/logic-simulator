/**
 * @file
 * @brief D-Latch Element
 */

#ifndef DIGISIM_ELEMENTS_D_LATCH_H
#define DIGISIM_ELEMENTS_D_LATCH_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * D Latch, for holding a data.
 *
 * To initialize/destroy, use di_d_latch_init/di_d_latch_destroy.
 */
typedef struct di_d_latch_t {
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
     * Data terminal, the value of this wire is connected to value when clock is set to 1.
     *
     * When clock goes to DI_BIT_LOW, `value` is latched to `data`.
     * Changing `data` will no longer change `value` until clock is set back to DI_BIT_HIGH.
     */
    DiTerminal data;

    /**
     * Clock terminal, setting to DI_BIT_HIGH will make this D Latch transparent.
     */
    DiTerminal clock;

    /**
     * The value currently contained in the bit.
     */
    DiTerminal value;
} DiDLatch;

/**
 * Initialize a DiDLatch struct.
 *
 * @memberof DiDLatch
 * @param latch Pointer to initialize
 * @param bits Number of bits stored in latch
 */
void di_d_latch_init(DiDLatch *latch, size_t bits);

/**
 * Destroy a DiDLatch struct.
 *
 * @memberof DiDLatch
 * @param latch Pointer to destroy
 */
void di_d_latch_destroy(DiDLatch *latch);

#endif // DIGISIM_ELEMENTS_D_LATCH_H
