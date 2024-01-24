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

void di_output_init(DiOutput *output, size_t bits) {
    di_element_init(&output->element);

    output->element.changed = di_output_changed;

    output->bits = bits;

    di_signal_init(&output->signal, bits);
    di_signal_fill(&output->signal, DI_BIT_LOW); // explicit

    di_terminal_init(&output->input, &output->element, bits);
}

void di_output_destroy(DiOutput *output) {
    di_element_destroy(&output->element);

    di_signal_destroy(&output->signal);
}

DiBit do_output_get_bit(DiOutput *output, size_t index) {
    return di_signal_get(&output->signal, index);
}
