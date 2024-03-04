#include <digisim/elements/not.h>

#include <digisim/node.h>

void di_not_changed(DiElement *component, DiSimulation *simulation) {
    DiNot *self = (DiNot *)component;

    DiSignal *in = di_terminal_read(&self->input);

    DiSignal *output = &self->output.signal;

    uint64_t *output_values = di_signal_get_values(output);
    uint64_t *output_error = di_signal_get_error(output);
    uint64_t *output_unknown = di_signal_get_unknown(output);

    uint64_t *in_values = di_signal_get_values(in);
    uint64_t *in_error = di_signal_get_error(in);
    uint64_t *in_unknown = di_signal_get_unknown(in);

    for (size_t a = 0; a < self->bits; a++) {
        size_t error = in_error[a];
        size_t unknown = in_unknown[a];
        size_t value = ~in_values[a] & ~error;

        output_error[a] = error;
        output_unknown[a] = unknown;
        output_values[a] = value;
    }

    di_terminal_send(&self->output, simulation);
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
