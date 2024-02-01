#include <digisim/signal.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DI_SIGNAL_U64_COUNT(bits)                                              \
    ((bits + 63) / 64 * DI_BIT_REPRESENTATION_SIZE)

DiBit di_bit_logical(bool value) {
    if (value) {
        return DI_BIT_HIGH;
    } else {
        return DI_BIT_LOW;
    }
}

bool di_bit_value(DiBit bit, bool default_value) {
    switch (bit) {
    case DI_BIT_LOW:
        return 0;
    case DI_BIT_HIGH:
        return 1;
    default:
        return default_value;
    }
}

DiBit di_signal_get(DiSignal *signal, size_t index) {
    assert(index < signal->bits);

    size_t i = index / DI_BITS_PER_U64;
    size_t off = index % DI_BITS_PER_U64;

    uint64_t *values = di_signal_values(signal);

    uint64_t value = values[i];

    return (value >> (off * DI_BIT_REPRESENTATION_SIZE)) &
           DI_BIT_REPRESENTATION_MASK;
}

void di_signal_set(DiSignal *signal, size_t index, DiBit bit) {
    assert(index < signal->bits);

    size_t i = index / DI_BITS_PER_U64;
    size_t off = index % DI_BITS_PER_U64;

    uint64_t *values = di_signal_values(signal);

    uint64_t value = values[i];

    value &= ~((size_t)DI_BIT_REPRESENTATION_MASK
               << (off * DI_BIT_REPRESENTATION_SIZE));
    value |= (uint64_t)(bit) << (off * DI_BIT_REPRESENTATION_SIZE);

    values[i] = value;
}

void di_signal_fill(DiSignal *signal, DiBit fill) {
    for (size_t a = 0; a < signal->bits; a++) {
        di_signal_set(signal, a, fill);

        assert(di_signal_get(signal, a) == fill);
    }
}

bool di_signal_equal(DiSignal *signal, DiSignal *other) {
    if (signal->bits != other->bits) {
        return false;
    }

    // We should probably use bitwise here.
    for (size_t a = 0; a < signal->bits; a++) {
        if (di_signal_get(signal, a) != di_signal_get(other, a)) {
            return false;
        }
    }

    return true;
}

void di_signal_copy(DiSignal *signal, DiSignal *source) {
    assert(signal->bits == source->bits);

    uint64_t *values = di_signal_values(signal);
    uint64_t *other = di_signal_values(source);

    memcpy(values, other, DI_SIGNAL_U64_COUNT(signal->bits) * sizeof(uint64_t));
}

uint64_t *di_signal_values(DiSignal *signal) {
    if (signal->bits > DI_SIGNAL_SMALL_SIZE) {
        return signal->heap;
    } else {
        return &signal->local;
    }
}

void di_signal_init(DiSignal *signal, size_t bits) {
    signal->bits = bits;

    if (bits > DI_SIGNAL_SMALL_SIZE) {
        size_t u64_required = DI_SIGNAL_U64_COUNT(bits);

        signal->heap = malloc(u64_required * sizeof(uint64_t));
        memset(signal->heap, 0, u64_required * sizeof(uint64_t));
    } else {
        signal->local = 0;
    }
}

void di_signal_init_from(DiSignal *signal, DiSignal *source) {
    di_signal_init(signal, source->bits);
    di_signal_copy(signal, source);
}

void di_signal_destroy(DiSignal *signal) {
    if (signal->bits > DI_SIGNAL_SMALL_SIZE) {
        free(signal->heap);
    }
}
