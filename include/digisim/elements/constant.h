/**
* @file
* @brief Constant Value Element (Ground and Power)
*/

#ifndef DIGISIM_ELEMENTS_CONSTANT_H
#define DIGISIM_ELEMENTS_CONSTANT_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * An element that always asserts a constant value onto its output.
 *
 * To initialize/destroy, use di_constant_init/di_constant_destroy.
 */
typedef struct di_constant_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Constant bit width.
     */
    size_t bits;

    /**
     * The constant value to be asserted onto the output.
     */
    uint64_t value;

    /**
     * Output terminal. The value on this terminal should be equal to `value`.
     */
    DiTerminal output;
} DiConstant;

// TODO: May need functions to modify value in the future.
// Right now this component lives up to its name of "constant."

/**
 * Initialize a DiConstant struct.
 *
 * @memberof DiConstant
 * @param constant Pointer to initialize
 * @param bits Number of bits in the constant (must be <= 64)
 * @param value The value of the constant (ex. 1 for Power, 0 for Ground, or some integer that fits in `bits`).
 */
void di_constant_init(DiConstant *constant, size_t bits, uint64_t value);

/**
 * Destroy a DiInput struct.
 *
 * @memberof DiConstant
 * @param input Pointer to destroy
 */
void di_constant_destroy(DiConstant *constant);

#endif // DIGISIM_ELEMENTS_CONSTANT_H
