/**
* @file
* @brief Utility for Printing Signals
*/

#ifndef DIGISIM_PRINT_H
#define DIGISIM_PRINT_H

#include <digisim/signal.h>

#include <stdio.h>

/**
 * Returns a character represting the bit.
 *
 * E.g. '0' for DI_BIT_LOW, '1' for DI_BIT_HIGH.
 * 'U' for DI_BIT_UNKNOWN, 'E' for DI_BIT_ERROR.
 *
 * @param bit The bit
 * @return A char representing the bit value
 */
char di_bit_to_char(DiBit bit);

/**
 * Formats a signals value into a character buffer.
 * MSB (highest index) is put first in the buffer.
 * A null terminator is NOT added, but exactly signal->bits characters are modified.
 *
 * @param signal The input signal to format.
 * @param output A buffer of signal->bits characters.
 */
void di_signal_format(DiSignal *signal, char *output);

/**
 * Prints a signal value to STDOUT.
 *
 * @param signal The signal to print.
 * @param terminator Appended to the input stream. A value of NULL is equivalent to "\n".
 */
void di_signal_print(DiSignal *signal, const char *terminator);

#endif // DIGISIM_PRINT_H
