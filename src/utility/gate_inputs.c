#include <digisim/utility/gate_inputs.h>

#include <assert.h>
#include <stdlib.h>

DiTerminal *di_gate_inputs_get(DiGateInputs *inputs, size_t index) {
    assert(index < inputs->count);

    if (inputs->count > DI_GATE_INPUTS_LOCAL_COUNT) {
        return &inputs->heap[index];
    } else {
        return &inputs->local[index];
    }
}

void di_gate_inputs_init(DiGateInputs *inputs, size_t count) {
    inputs->count = count;

    if (count > DI_GATE_INPUTS_LOCAL_COUNT) {
        inputs->heap = malloc(count * sizeof(DiTerminal));
    }
}

void di_gate_inputs_destroy(DiGateInputs *inputs) {
    if (inputs->count > DI_GATE_INPUTS_LOCAL_COUNT) {
        free(inputs->heap);
    }
}
