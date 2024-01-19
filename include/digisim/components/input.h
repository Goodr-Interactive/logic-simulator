#pragma once

#include <digisim/component.h>
#include <digisim/connection.h>

typedef struct di_input_t {
    DiComponent component;

    size_t bits;
    DiSignal signal;

    DiConnection output;
} DiInput;

void di_input_init(DiInput *input, size_t bits);
void di_input_destroy(DiInput *input);

DiBit di_input_get_bit(DiInput *input, size_t index);
void di_input_set_bit(DiInput *input, size_t index, DiBit bit);
void di_input_set(DiInput *input, DiSignal move_signal);
