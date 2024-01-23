#include <digisim/elements/output.h>

void di_output_changed(DiElement *component) {
    DiOutput *output = (DiOutput *)component;

    DiSignal *signal = di_terminal_read(&output->input);

    if (!signal) {
        di_signal_fill(&output->signal, DI_BIT_ERROR);

        return;
    }

    // Check to make sure bits are okay. We need good error propagation.

    di_signal_copy(&output->signal, signal);
}

void di_output_init(DiOutput *input, size_t bits) {
    DiElement component = {
        .retain = NULL,
        .release = NULL,
        .changed = di_output_changed,
    };

    di_element_init(&component);
    input->element = component;

    input->bits = bits;

    di_signal_init(&input->signal, bits);
    di_signal_fill(&input->signal, DI_BIT_LOW); // explicit

    di_terminal_init(&input->input, &input->element, bits);
}

void di_output_destroy(DiOutput *input) {
    di_element_destroy(&input->element);

    di_signal_destroy(&input->signal);
}

DiBit do_output_get_bit(DiOutput *input, size_t index) {
    return di_signal_get(&input->signal, index);
}
