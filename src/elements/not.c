#include <digisim/elements/not.h>

#include <digisim/node.h>

void di_not_changed(DiElement *component, DiSimulation *simulation) {
    DiNot *self = (DiNot *)component;

    DiSignal *signal = di_terminal_read(&self->input);

    if (!signal) {
        di_terminal_reset(&self->output, simulation);

        return;
    }

    DiSignal output;
    di_signal_init(&output, self->bits);

    for (size_t a = 0; a < self->bits; a++) {
        bool value = di_bit_value(di_signal_get(signal, a), false);

        di_signal_set(&output, a, di_bit_logical(!value));
    }

    di_terminal_write(&self->output, output, simulation);
}

void di_not_init(DiNot *self, size_t bits) {
    di_element_init(&self->element);

    self->element.changed = di_not_changed;
    self->bits = bits;

    di_terminal_init(&self->input, &self->element, bits);
    di_terminal_init(&self->output, &self->element, bits);
}

void di_not_destroy(DiNot *self) {
    di_terminal_destroy(&self->input);
    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
