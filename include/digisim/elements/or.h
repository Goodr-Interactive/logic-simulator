#pragma once

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_or_t {
    DiElement element;

    size_t bits;

    DiTerminal input_a;
    DiTerminal input_b;

    DiTerminal output;
} DiOr;

void di_or_init(DiOr *self, size_t bits);
void di_or_destroy(DiOr *self);
