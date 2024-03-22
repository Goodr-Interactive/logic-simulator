#ifndef DIGISIM_INPUT_LIST_H
#define DIGISIM_INPUT_LIST_H

#include <digisim/terminal.h>

#define DI_GATE_INPUTS_LOCAL_COUNT 2

typedef struct di_gate_inputs_t {
    size_t count;

    union {
        DiTerminal local[DI_GATE_INPUTS_LOCAL_COUNT];

        DiTerminal *heap;
    };
} DiGateInputs;

DiTerminal *di_gate_inputs_get(DiGateInputs *inputs, size_t index);

void di_gate_inputs_init(DiGateInputs *inputs, size_t count);
void di_gate_inputs_destroy(DiGateInputs *inputs);

#endif // DIGISIM_INPUT_LIST_H
