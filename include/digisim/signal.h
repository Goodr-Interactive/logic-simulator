/**
 * @file
 * @brief Interface for Wire and Terminal Values
 */

#ifndef DIGISIM_SIGNAL_H
#define DIGISIM_SIGNAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * Number of DiBits that can fit in one u64.
 */
#define DI_SIGNAL_BITS_PER_U64 64

/**
 * Locally kept capacity for DiSignal.
 */
#define DI_SIGNAL_SMALL_SIZE DI_SIGNAL_BITS_PER_U64

/**
 * Number of u64 values in a DiSignal with this amount of bits.
 */
#define DI_SIGNAL_U64_COUNT(bits) ((bits + DI_SIGNAL_BITS_PER_U64 - 1) / DI_SIGNAL_BITS_PER_U64)

/**
 * Number of u64 values in a DiSignal with this amount of bits.
 */
#define DI_SIGNAL_U64_ALLOC(bits) (3 * DI_SIGNAL_U64_COUNT(bits))

#define DI_SIGNAL_ERROR_OFFSET(bits) DI_SIGNAL_U64_COUNT(bits)
#define DI_SIGNAL_UNKNOWN_OFFSET(bits) (2 * DI_SIGNAL_U64_COUNT(bits))

/**
 * Represents one bit on a multi-bit wire.
 */
typedef enum di_bit_t {
    /**
     * Low, ground, logical zero...
     */
    DI_BIT_LOW = 0b00,

    /**
     * High, Vcc, logical one...
     */
    DI_BIT_HIGH = 0b01,

    /**
     * Mix of logical one and zero, invalid circuit input...
     */
    DI_BIT_ERROR = 0b10,

    /**
     * Unconnected wire, disabled buffer output...
     */
    DI_BIT_UNKNOWN = 0b11,
} DiBit;

/**
 * Represents a multi-bit value held on a wire.
 *
 * Packs DiBit enums in an array of u64s.
 * Use `di_signal_get`/`di_signal_set` to access bits.
 *
 * To initialize/destroy, use di_signal_init/di_signal_destroy.
 */
typedef struct di_signal_t {
    /**
     * The size of the wire signal in bits.
     */
    size_t bits;

    union {
        /**
         * Locally held storage for bits. Used if `bits <= DI_SIGNAL_SMALL_SIZE`.
         */
        uint64_t local[3];

        /**
         * Heap allocated storage for bits. Used if `bits > DI_SIGNAL_SMALL_SIZE`.
         */
        uint64_t *heap;
    };
} DiSignal;

uint64_t *di_signal_get_values(DiSignal *signal);
uint64_t *di_signal_get_error(DiSignal *signal);
uint64_t *di_signal_get_unknown(DiSignal *signal);

/**
 * Returns `DI_BIT_HIGH` if value is true, `DI_BIT_LOW` otherwise.
 *
 * @param value Logical bit selector
 * @return DiBit corresponding logically to `value`
 */
DiBit di_bit_logical(bool value);

/**
 * Returns `true` if `bit` is `DI_BIT_HIGH`, and `false` if `bit` is `DI_BIT_LOW`.
 * Otherwise, returns `default_value`.
 *
 * @param bit The bit selector
 * @param default_value The default value for unknown/error bits
 * @return The logical value of the bit
 */
bool di_bit_value(DiBit bit, bool default_value);

/**
 * The bit at index `index` in a signal.
 *
 * @memberof DiSignal
 * @param signal The signal to access
 * @param index The index of a bit to get (`index < signal->bits`)
 * @return The bit inside the signal at this index
 */
DiBit di_signal_get(DiSignal *signal, size_t index);

/**
 * Changes a bit at index `index` in the signal.
 *
 * @memberof DiSignal
 * @param signal The signal to modify
 * @param index The index of a bit to change (`index < signal->bits`)
 * @param bit The new value of the bit at `index`
 */
void di_signal_set(DiSignal *signal, size_t index, DiBit bit);

/**
 * Changes every bit in the signal to be `bit`
 *
 * @memberof DiSignal
 * @param signal The signal to modify
 * @param bit The new value for every bit in the signal
 */
void di_signal_fill(DiSignal *signal, DiBit bit);

/**
 * Creates a signal with a certain amount of bits, all set to `bit`.
 * Equivalent to di_signal_init and di_signal_fill `bit`
 *
 * @param bit The bit to fill the signal
 * @param bits The amount of bits in the returned signal.
 * @return A signal where all bits are set to DI_BIT_ERROR.
 */
DiSignal di_signal_filled(size_t bits, DiBit bit);

/**
 * Compares the values of two signals.
 * Returns true if both signals are the same length and every bit in the first signal is equal to the second signal.
 *
 * @memberof DiSignal
 * @param signal The first signal
 * @param other The second signal
 * @return If both signals are equal in value
 */
bool di_signal_equal(DiSignal *signal, DiSignal *other);

/**
 * Overwrites the value of `signal` with the value at `source`.
 * The amount of bits in both signals must be the same (`signal->bits == source->bits`).
 *
 * @memberof DiSignal
 * @param signal The signal to modify/replace
 * @param source The signal to acquire the bits from
 */
void di_signal_copy(DiSignal *signal, DiSignal *source);

/**
 * Initialize a DiSignal struct.
 * All bits inside the signal will be set to zero.
 *
 * @memberof DiSignal
 * @param signal Pointer to initialize
 * @param bits The number of bits the signal contains
 */
void di_signal_init(DiSignal *signal, size_t bits);

/**
 * Initialize a DiSignal struct with the number of bits and values of `source` signal.
 * This is equivalent to calling `di_signal_init` then `di_signal_copy`.
 *
 * @memberof DiSignal
 * @param signal Pointer to initialize
 * @param source The source to copy size and content from
 */
void di_signal_init_from(DiSignal *signal, DiSignal *source);

/**
 * Destroy a DiSignal struct.
 *
 * @memberof DiSignal
 * @param signal Pointer to destroy
 */
void di_signal_destroy(DiSignal *signal);

#endif // DIGISIM_SIGNAL_H
