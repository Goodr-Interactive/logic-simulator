#include <digisim/components/and.h>

#include <digisim/wire.h>

void di_and_changed(DiComponent *component) {
    DiAnd *self = (DiAnd *) component;

    DiSignal *in_a = di_connection_read(&self->input_a);
    DiSignal *in_b = di_connection_read(&self->input_b);

    if (!in_a || !in_b) {
        di_connection_reset(&self->output);

        return;
    }

    DiSignal output;
    di_signal_init(&output, self->bits);

    for (size_t a = 0; a < self->bits; a++) {
        bool lhs = di_bit_value(di_signal_get(in_a, a), false);
        bool rhs = di_bit_value(di_signal_get(in_b, a), false);

        di_signal_set(&output, a, di_bit_logical(lhs && rhs));
    }

    di_connection_write(&self->output, output);
}

void di_and_init(DiAnd *self, size_t bits) {
    DiComponent component = {
        .retain = NULL,
        .release = NULL,
        .changed = di_and_changed
    };

    di_component_init(&component);

    self->component = component;
    self->bits = bits;

    di_connection_init(&self->input_a, &self->component, bits);
    di_connection_init(&self->input_b, &self->component, bits);

    di_connection_init(&self->output, &self->component, bits);
}

void di_and_destroy(DiAnd *self) {
    di_component_destroy(&self->component);
}
