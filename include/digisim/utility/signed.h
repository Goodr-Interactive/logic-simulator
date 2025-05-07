#ifndef DIGISIM_SIGNED_H
#define DIGISIM_SIGNED_H

#include <stdint.h>
#include <stddef.h>

#define DI_SIGNED_TOP_MASK(bits) ((~(uint64_t)0) << (bits))

int64_t di_signed_make(uint64_t value, size_t bits);

#endif // DIGISIM_SIGNED_H
