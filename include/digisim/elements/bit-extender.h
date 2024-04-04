/**
* @file
* @brief Bit Extender Element
*/

#ifndef DIGISIM_ELEMENTS_BIT_EXTENDER_H
#define DIGISIM_ELEMENTS_BIT_EXTENDER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Policy for bit extension.
 * Determines the value for the "extended" bits.
 */
typedef enum di_bit_extender_policy_t {
    /**
     * Extended bits are set to zero.
     */
    DI_BIT_EXTENDER_POLICY_ZERO,
    /**
     * Extended bits are set to one.
     */
    DI_BIT_EXTENDER_POLICY_ONE,
    /**
     * Extended bits are set to MSB.
     * For signed bit extension.
     */
    DI_BIT_EXTENDER_POLICY_SIGN,
} DiBitExtenderPolicy;

/**
 * Bit extender element, for converting a smaller bit size wire to a larger bit size.
 *
 * To initialize/destroy, use di_bit_extender_init/di_bit_extender_destroy.
 */
typedef struct di_bit_extender_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * The policy to determine the values of the extended bits.
     */
    DiBitExtenderPolicy policy;

    /**
     * The number of input bits.
     */
    size_t in_bits;
    /**
     * The number of output bits (after extension).
     */
    size_t out_bits;

    /**
     * The input terminal. The bit width for this terminal is `in_bits`.
     */
    DiTerminal input;

    /**
     * The output terminal. The bit width for this terminal is `out_bits`.
     */
    DiTerminal output;
} DiBitExtender;

/**
 * Initialize a DiBitExtender struct.
 *
 * `in_bits` must be less than `out_bits`.
 *
 * @memberof DiBitExtender
 * @param output Pointer to initialize
 * @param policy Determines the value of the extended bits
 * @param in_bits The number of bits on the signal to extend.
 * @param out_bits The number of bits on the extended signal (after extension).
 */
void di_bit_extender_init(DiBitExtender *extender, DiBitExtenderPolicy policy, size_t in_bits, size_t out_bits);

/**
 * Destroy a DiBitExtender struct.
 *
 * @memberof DiBitExtender
 * @param output Pointer to destroy
 */
void di_bit_extender_destroy(DiBitExtender *extender);

#endif // DIGISIM_ELEMENTS_BIT_EXTENDER_H
