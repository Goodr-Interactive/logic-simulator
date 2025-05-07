#include <digisim/elements/input.h>

#include <digisim/simulation.h>

void di_input_changed(DiElement *element, DiSimulation *simulation) {
    DiInput *input = (DiInput *)element;

    di_input_emit(input, simulation);
}

void di_input_init(DiInput *input, size_t bits) {
    di_element_init(&input->element);

    input->element.changed = di_input_changed;

    input->bits = bits;

    di_signal_init(&input->signal, bits);
    di_signal_fill(&input->signal, DI_BIT_LOW); // explicit

    di_terminal_init(&input->output, &input->element, bits);
}

void di_input_destroy(DiInput *input) {
    di_terminal_destroy(&input->output);

    di_element_destroy(&input->element);

    di_signal_destroy(&input->signal);
}

void di_input_emit(DiInput *input, DiSimulation *simulation) {
    di_signal_copy(&input->output.signal, &input->signal);

    di_terminal_send(&input->output, simulation);
}

DiBit di_input_get_bit(DiInput *input, size_t index) { return di_signal_get(&input->signal, index); }

void di_input_set_bit(DiInput *input, size_t index, DiBit bit, DiSimulation *simulation) {
    di_signal_set(&input->signal, index, bit);

    di_input_emit(input, simulation);
}

void di_input_set(DiInput *input, DiSignal move_signal, DiSimulation *simulation) {
    di_signal_destroy(&input->signal);

    input->signal = move_signal;

    di_input_emit(input, simulation);
}
