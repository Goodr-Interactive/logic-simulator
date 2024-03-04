#include <digisim/elements/xnor.h>

#include <digisim/node.h>

void di_xnor_changed(DiElement *component, DiSimulation *simulation) {
    DiXnor *self = (DiXnor *)component;

    DiSignal *in_a = di_terminal_read(&self->input_a);
    DiSignal *in_b = di_terminal_read(&self->input_b);

    DiSignal *output = &self->output.signal;

    uint64_t *output_values = di_signal_get_values(output);
    uint64_t *output_error = di_signal_get_error(output);
    uint64_t *output_unknown = di_signal_get_unknown(output);

    uint64_t *in_a_values = di_signal_get_values(in_a);
    uint64_t *in_a_error = di_signal_get_error(in_a);
    uint64_t *in_a_unknown = di_signal_get_unknown(in_a);

    uint64_t *in_b_values = di_signal_get_values(in_b);
    uint64_t *in_b_error = di_signal_get_error(in_b);
    uint64_t *in_b_unknown = di_signal_get_unknown(in_b);

    for (size_t a = 0; a < self->bits; a++) {
        size_t error = in_a_error[a] | in_b_error[a];
        size_t unknown = in_a_unknown[a] & in_b_unknown[a];
        size_t value = ~(in_a_values[a] ^ in_b_values[a]) & ~error;

        output_error[a] = error;
        output_unknown[a] = unknown;
        output_values[a] = value;
    }

    di_terminal_send(&self->output, simulation);
}

void di_xnor_init(DiXnor *self, size_t bits) {
    di_element_init(&self->element);

    self->element.changed = di_xnor_changed;

    self->bits = bits;

    di_terminal_init(&self->input_a, &self->element, bits);
    di_terminal_init(&self->input_b, &self->element, bits);

    di_terminal_init(&self->output, &self->element, bits);
}

void di_xnor_destroy(DiXnor *self) {
    di_terminal_destroy(&self->input_a);
    di_terminal_destroy(&self->input_b);
    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
