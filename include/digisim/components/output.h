#pragma once

#include <digisim/component.h>
#include <digisim/connection.h>

typedef struct di_output_t {
    DiComponent component;

    size_t bits;
    DiSignal signal;

    DiConnection input;
} DiOutput;

void di_output_init(DiOutput *input, size_t bits);
void di_output_destroy(DiOutput *input);

DiBit di_output_get_bit(DiOutput *input, size_t index);
