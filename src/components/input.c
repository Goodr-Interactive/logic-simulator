#include <digisim/elements/input.h>

void di_input_init(DiInput *input, size_t bits) {
    DiElement component = {
        .retain = NULL,
        .release = NULL,
        .changed = NULL,
    };

    di_element_init(&component);
    input->element = component;

    input->bits = bits;

    di_signal_init(&input->signal, bits);
    di_signal_fill(&input->signal, DI_BIT_LOW); // explicit

    di_terminal_init(&input->output, &input->element, bits);
}

void di_input_destroy(DiInput *input) {
    di_element_destroy(&input->element);

    di_signal_destroy(&input->signal);
}

DiBit di_input_get_bit(DiInput *input, size_t index) {
    return di_signal_get(&input->signal, index);
}

void di_input_emit(DiInput *input) {
    DiSignal copy;
    di_signal_init_from(&copy, &input->signal);

    di_terminal_write(&input->output, copy);
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
