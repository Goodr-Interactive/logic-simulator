#pragma once

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_input_t {
    DiElement element;

    size_t bits;
    DiSignal signal;

    DiTerminal output;
} DiInput;

void di_input_init(DiInput *input, size_t bits);
void di_input_destroy(DiInput *input);

void di_input_emit(DiInput *input, DiSimulation *simulation);

DiBit di_input_get_bit(DiInput *input, size_t index);
void di_input_set_bit(DiInput *input, size_t index, DiBit bit, DiSimulation *simulation);
void di_input_set(DiInput *input, DiSignal move_signal, DiSimulation *simulation);
