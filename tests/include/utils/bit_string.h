#ifndef DIGISIM_BIT_STRING_H
#define DIGISIM_BIT_STRING_H

#include <string>

extern "C" {
#include <digisim/signal.h>
}

namespace bit_string {
DiBit char_to_bit(char c);
char bit_to_char(DiBit c);
void copy_bit_string_to_signal(DiSignal *signal, const std::string &bits);
std::string signal_to_bit_string(DiSignal *signal);

// RAII Wrapped DiSignal
struct BitString {
    DiSignal signal {};

    explicit BitString(const std::string &bits);
    ~BitString();
};
}

#endif // DIGISIM_BIT_STRING_H
