#include <digisim/signal.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

uint64_t *di_signal_get_values(DiSignal *signal) {
    if (signal->bits > DI_SIGNAL_SMALL_SIZE) {
        return signal->heap;
    } else {
        return signal->local;
    }
}

uint64_t *di_signal_get_error(DiSignal *signal) {
    return &di_signal_get_values(signal)[DI_SIGNAL_U64_COUNT(signal->bits)];
}

uint64_t *di_signal_get_unknown(DiSignal *signal) {
    return &di_signal_get_values(signal)[2 * DI_SIGNAL_U64_COUNT(signal->bits)];
}

DiBit di_signal_get(DiSignal *signal, size_t index) {
    assert(index < signal->bits);

    size_t i = index / DI_SIGNAL_BITS_PER_U64;
    size_t off = index % DI_SIGNAL_BITS_PER_U64;

    uint64_t mask = ((uint64_t)(1) << off);

    if ((di_signal_get_error(signal)[i] & mask) != 0) {
        return DI_BIT_ERROR;
    }

    if ((di_signal_get_unknown(signal)[i] & mask) != 0) {
        return DI_BIT_UNKNOWN;
    }

    if ((di_signal_get_values(signal)[i] & mask) != 0) {
        return DI_BIT_HIGH;
    } else {
        return DI_BIT_LOW;
    }
}

void di_signal_set(DiSignal *signal, size_t index, DiBit bit) {
    assert(index < signal->bits);

    size_t i = index / DI_SIGNAL_BITS_PER_U64;
    size_t off = index % DI_SIGNAL_BITS_PER_U64;

    uint64_t mask = ~((uint64_t)(1) << off);

    uint64_t *values = di_signal_get_values(signal);

    size_t error_i = DI_SIGNAL_ERROR_OFFSET(signal->bits) + i;
    size_t unknown_i = DI_SIGNAL_UNKNOWN_OFFSET(signal->bits) + i;

    values[i] = (values[i] & mask) | ((uint64_t)(bit == DI_BIT_HIGH) << off);
    values[error_i] = (values[error_i] & mask) | ((uint64_t)(bit == DI_BIT_ERROR) << off);
    values[unknown_i] = (values[unknown_i] & mask) | ((uint64_t)(bit == DI_BIT_UNKNOWN) << off);
}

void di_signal_fill(DiSignal *signal, DiBit fill) {
    for (size_t a = 0; a < signal->bits; a++) {
        di_signal_set(signal, a, fill);

        assert(di_signal_get(signal, a) == fill);
    }
}

DiSignal di_signal_filled(size_t bits, DiBit bit) {
    DiSignal signal;

    di_signal_init(&signal, bits);
    di_signal_fill(&signal, bit);

    return signal;
}

bool di_signal_equal(DiSignal *signal, DiSignal *other) {
    if (signal->bits != other->bits) {
        return false;
    }

    // Assuming unused bits are zero.
    uint64_t *signal_values = di_signal_get_values(signal);
    uint64_t *other_values = di_signal_get_values(other);

    return memcmp(signal_values, other_values, sizeof(uint64_t) * DI_SIGNAL_U64_ALLOC(signal->bits)) == 0;
}

void di_signal_copy(DiSignal *signal, DiSignal *source) {
    assert(signal->bits == source->bits);

    size_t byte_count = DI_SIGNAL_U64_ALLOC(signal->bits) * sizeof(uint64_t);

    // Assuming signal->values is the base for error and unknown.
    memcpy(di_signal_get_values(signal), di_signal_get_values(source), byte_count);
}

void di_signal_init(DiSignal *signal, size_t bits) {
    signal->bits = bits;

    if (bits > DI_SIGNAL_SMALL_SIZE) {
        size_t u64_required = DI_SIGNAL_U64_ALLOC(bits); // values + error + unknown

        signal->heap = malloc(u64_required * sizeof(uint64_t));
        memset(signal->heap, 0, u64_required * sizeof(uint64_t));
    } else {
        memset(signal->local, 0, sizeof(signal->local));
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
