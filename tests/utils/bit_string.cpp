#include <utils/bit_string.h>

#include <cassert>

namespace bit_string {
DiBit char_to_bit(char c) {
    switch (c) {
    case '0':
        return DI_BIT_LOW;
    case '1':
        return DI_BIT_HIGH;
    case 'U':
        return DI_BIT_UNKNOWN;
    case 'E':
        return DI_BIT_ERROR;
    default:
        return DI_BIT_LOW;
    }
}

char bit_to_char(DiBit c) {
    switch (c) {
    case DI_BIT_LOW: return '0';
    case DI_BIT_HIGH: return '1';
    case DI_BIT_ERROR: return 'E';
    case DI_BIT_UNKNOWN: return 'U';
    }
}

void copy_bit_string_to_signal(DiSignal *signal, const std::string &bits) {
    assert(bits.size() == signal->bits);

    for (size_t a = 0; a < bits.size(); a++) {
        di_signal_set(signal, bits.size() - 1 - a, char_to_bit(bits[a]));
    }
}

std::string signal_to_bit_string(DiSignal *signal) {
    std::string result;

    result.reserve(signal->bits);

    for (size_t a = 0; a < signal->bits; a++) {
        result += bit_to_char(di_signal_get(signal, signal->bits - 1 - a));
    }

    return result;
}

BitString::BitString(const std::string &bits) {
    di_signal_init(&signal, bits.size());

    copy_bit_string_to_signal(&signal, bits);
}

BitString::~BitString() {
    di_signal_destroy(&signal);
}
}
