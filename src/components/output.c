#include <digisim/components/output.h>

void di_output_changed(DiComponent *component) {
    DiOutput *output = (DiOutput *)component;

    DiSignal *signal = di_connection_read(&output->input);

    if (!signal) {
        di_signal_fill(&output->signal, DI_BIT_ERROR);

        return;
    }

    // Check to make sure bits are okay. We need good error propagation.

    di_signal_copy(&output->signal, signal);
}

void di_output_init(DiOutput *input, size_t bits) {
    DiComponent component = {
        .retain = NULL,
        .release = NULL,
        .changed = di_output_changed,
    };

    di_component_init(&component);
    input->component = component;

    input->bits = bits;

    di_signal_init(&input->signal, bits);
    di_signal_fill(&input->signal, DI_BIT_LOW); // explicit

    di_connection_init(&input->input, &input->component, bits);
}

void di_output_destroy(DiOutput *input) {
    di_component_destroy(&input->component);

    di_signal_destroy(&input->signal);
}

DiBit do_output_get_bit(DiOutput *input, size_t index) {
    return di_signal_get(&input->signal, index);
}
