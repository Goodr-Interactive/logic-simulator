#include <digisim/elements/and.h>

#include <digisim/node.h>

void di_and_changed(DiElement *component) {
    DiAnd *self = (DiAnd *) component;

    DiSignal *in_a = di_terminal_read(&self->input_a);
    DiSignal *in_b = di_terminal_read(&self->input_b);

    if (!in_a || !in_b) {
        di_terminal_reset(&self->output);

        return;
    }

    DiSignal output;
    di_signal_init(&output, self->bits);

    for (size_t a = 0; a < self->bits; a++) {
        bool lhs = di_bit_value(di_signal_get(in_a, a), false);
        bool rhs = di_bit_value(di_signal_get(in_b, a), false);

        di_signal_set(&output, a, di_bit_logical(lhs && rhs));
    }

    di_terminal_write(&self->output, output);
}

void di_and_init(DiAnd *self, size_t bits) {
    DiElement component = {
        .retain = NULL,
        .release = NULL,
        .changed = di_and_changed
    };

    di_element_init(&component);

    self->element = component;
    self->bits = bits;

    di_terminal_init(&self->input_a, &self->element, bits);
    di_terminal_init(&self->input_b, &self->element, bits);

    di_terminal_init(&self->output, &self->element, bits);
}

void di_and_destroy(DiAnd *self) {
    di_element_destroy(&self->element);
}
