#ifndef DIGISIM_CONSTANT_H
#define DIGISIM_CONSTANT_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_constant_t {
    DiElement element;

    size_t bits;
    uint64_t value;

    DiTerminal output;
} DiConstant;

void di_constant_init(DiConstant *constant, size_t bits, uint64_t value);
void di_constant_destroy(DiConstant *constant);

#endif // DIGISIM_CONSTANT_H
