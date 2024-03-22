#ifndef DIGISIM_SIGNED_H
#define DIGISIM_SIGNED_H

#include <stdint.h>
#include <stddef.h>

#define DI_SIGNED_TOP_MASK(bits) ((~(uint64_t)0) << (bits))

inline int64_t di_signed_make(uint64_t value, size_t bits) {
    uint64_t mask = 1 << (bits - 1);

    if ((mask & value) != 0) {
        // Two's complement casting.
        // Set the sign bits for the native system to interpret this properly.
        return (int64_t)(value | DI_SIGNED_TOP_MASK(bits));
    } else {
        return (int64_t)value;
    }
}

#endif // DIGISIM_SIGNED_H
