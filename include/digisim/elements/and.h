#pragma once

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_and_t {
    DiElement element;

    size_t bits;

    DiTerminal input_a;
    DiTerminal input_b;

    DiTerminal output;
} DiAnd;

void di_and_init(DiAnd *self, size_t bits);
void di_and_destroy(DiAnd *self);
