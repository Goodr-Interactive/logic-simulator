#ifndef DIGISIM_ELEMENTS_COMPARATOR_H
#define DIGISIM_ELEMENTS_COMPARATOR_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_comparator_t {
    DiElement element;

    size_t bits;
    bool is_signed;

    DiTerminal input_a;
    DiTerminal input_b;

    DiTerminal greater;
    DiTerminal equal;
    DiTerminal less;
} DiComparator;

void di_comparator_init(DiComparator *self, size_t bits, bool is_signed);
void di_comparator_destroy(DiComparator *self);

#endif // DIGISIM_ELEMENTS_COMPARATOR_H
