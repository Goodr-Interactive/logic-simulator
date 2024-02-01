/**
 * @file
 * @brief Interface for Wire and Terminal Values
 */

#ifndef DIGISIM_SIGNAL_H
#define DIGISIM_SIGNAL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define DI_BIT_REPRESENTATION_SIZE 2
#define DI_BIT_REPRESENTATION_MASK 0b11
#define DI_BITS_PER_U64 (64 / DI_BIT_REPRESENTATION_SIZE)
#define DI_SIGNAL_SMALL_SIZE DI_BITS_PER_U64

typedef enum di_bit_t {
    DI_BIT_LOW = 0b00,
    DI_BIT_UNKNOWN = 0b01,
    DI_BIT_ERROR = 0b10,
    DI_BIT_HIGH = 0b11,
} DiBit;

typedef struct di_signal_t {
    size_t bits;

    union {
        uint64_t local;
        uint64_t *heap;
    };
} DiSignal;

DiBit di_bit_logical(bool value);
bool di_bit_value(DiBit bit, bool default_value);

DiBit di_signal_get(DiSignal *signal, size_t index);
void di_signal_set(DiSignal *signal, size_t index, DiBit bit);

void di_signal_fill(DiSignal *signal, DiBit bit);

bool di_signal_equal(DiSignal *signal, DiSignal *other);
void di_signal_copy(DiSignal *signal, DiSignal *source);

uint64_t *di_signal_values(DiSignal *signal);

void di_signal_init(DiSignal *signal, size_t bits);
void di_signal_init_from(DiSignal *signal, DiSignal *source);
void di_signal_destroy(DiSignal *signal);

#endif // DIGISIM_SIGNAL_H
