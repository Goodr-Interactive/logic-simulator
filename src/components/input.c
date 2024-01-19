#include <digisim/components/input.h>

void di_input_init(DiInput *input, size_t bits) {
    DiComponent component = {
        .retain = NULL,
        .release = NULL,
        .changed = NULL,
    };

    di_component_init(&component);
    input->component = component;

    input->bits = bits;

    di_signal_init(&input->signal, bits);
    di_signal_fill(&input->signal, DI_BIT_LOW); // explicit

    di_connection_init(&input->output, &input->component, bits);
}

void di_input_destroy(DiInput *input) {
    di_component_destroy(&input->component);

    di_signal_destroy(&input->signal);
}

DiBit di_input_get_bit(DiInput *input, size_t index) {
    return di_signal_get(&input->signal, index);
}

void di_input_emit(DiInput *input) {
    DiSignal copy;
    di_signal_init_from(&copy, &input->signal);

    di_connection_write(&input->output, copy);
}

void di_input_set_bit(DiInput *input, size_t index, DiBit bit) {
    di_signal_set(&input->signal, index, bit);

    di_input_emit(input);
}

void di_input_set(DiInput *input, DiSignal move_signal) {
    di_signal_destroy(&input->signal);

    input->signal = move_signal;

    di_input_emit(input);
}
