#ifndef DIGISIM_ELEMENTS_NEGATE_H
#define DIGISIM_ELEMENTS_NEGATE_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_negate_t {
    DiElement element;

    size_t bits;

    DiTerminal input;
    DiTerminal output;
} DiNegate;

void di_negate_init(DiNegate *negate, size_t bits);
void di_negate_destroy(DiNegate *negate);

#endif // DIGISIM_ELEMENTS_NEGATE_H
