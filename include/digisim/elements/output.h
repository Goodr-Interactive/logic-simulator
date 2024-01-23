#pragma once

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_output_t {
    DiElement element;

    size_t bits;
    DiSignal signal;

    DiTerminal input;
} DiOutput;

void di_output_init(DiOutput *input, size_t bits);
void di_output_destroy(DiOutput *input);

DiBit di_output_get_bit(DiOutput *input, size_t index);
