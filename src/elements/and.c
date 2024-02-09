#include <digisim/elements/and.h>

#include <digisim/node.h>

void di_and_changed(DiElement *component, DiSimulation *simulation) {
    DiAnd *self = (DiAnd *)component;

    DiSignal *in_a = di_terminal_read(&self->input_a);
    DiSignal *in_b = di_terminal_read(&self->input_b);

    if (!in_a || !in_b) {
        di_terminal_reset(&self->output, simulation);

        return;
    }

    DiSignal output;
    di_signal_init(&output, self->bits);

    for (size_t a = 0; a < self->bits; a++) {
        bool lhs = di_bit_value(di_signal_get(in_a, a), false);
        bool rhs = di_bit_value(di_signal_get(in_b, a), false);

        di_signal_set(&output, a, di_bit_logical(lhs && rhs));
    }

    di_terminal_write(&self->output, output, simulation);
}

void di_and_init(DiAnd *self, size_t bits) {
    di_element_init(&self->element);

    self->element.changed = di_and_changed;

    self->bits = bits;

    di_terminal_init(&self->input_a, &self->element, bits);
    di_terminal_init(&self->input_b, &self->element, bits);

    di_terminal_init(&self->output, &self->element, bits);
}

void di_and_destroy(DiAnd *self) {
    di_terminal_destroy(&self->input_a);
    di_terminal_destroy(&self->input_b);
    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
