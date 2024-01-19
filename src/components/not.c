#include <digisim/components/not.h>

#include <digisim/wire.h>

void di_not_changed(DiComponent *component) {
    DiNot *self = (DiNot *)component;

    DiSignal *signal = di_connection_read(&self->input);

    if (!signal) {
        di_connection_reset(&self->output);

        return;
    }

    DiSignal output;
    di_signal_init(&output, self->bits);

    for (size_t a = 0; a < self->bits; a++) {
        bool value = di_bit_value(di_signal_get(signal, a), false);

        di_signal_set(&output, a, di_bit_logical(!value));
    }

    di_connection_write(&self->output, output);
}

void di_not_init(DiNot *self, size_t bits) {
    DiComponent component = {
        .retain = NULL,
        .release = NULL,
        .changed = di_not_changed
    };

    di_component_init(&component);

    self->component = component;
    self->bits = bits;

    di_connection_init(&self->input, &self->component, bits);
    di_connection_init(&self->output, &self->component, bits);
}

void di_not_destroy(DiNot *self) {
    di_component_destroy(&self->component);
}
