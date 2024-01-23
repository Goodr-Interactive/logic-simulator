#pragma once

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_not_t {
    DiElement element;

    size_t bits;

    DiTerminal input;
    DiTerminal output;
} DiNot;

void di_not_init(DiNot *self, size_t bits);
void di_not_destroy(DiNot *self);
