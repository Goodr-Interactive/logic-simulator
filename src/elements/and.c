#include <digisim/elements/and.h>

#include <digisim/node.h>

#include <assert.h>

void di_and_changed(DiElement *component, DiSimulation *simulation) {
    DiAnd *self = (DiAnd *)component;

    DiSignal *output = &self->output.signal;

    di_signal_copy(output, di_terminal_read(di_gate_inputs_get(&self->inputs, 0)));

    uint64_t *output_values = di_signal_get_values(output);
    uint64_t *output_error = di_signal_get_error(output);
    uint64_t *output_unknown = di_signal_get_unknown(output);

    for (size_t i = 1; i < self->inputs.count; i++) {
        DiSignal *input = di_terminal_read(di_gate_inputs_get(&self->inputs, i));

        uint64_t *in_values = di_signal_get_values(input);
        uint64_t *in_error = di_signal_get_error(input);
        uint64_t *in_unknown = di_signal_get_unknown(input);

        for (size_t a = 0; a < self->bits; a++) {
            size_t error = output_error[a] | in_error[a];
            size_t unknown = output_unknown[a] & in_unknown[a];
            size_t value = (output_values[a] & in_values[a]) & ~error;

            output_error[a] = error;
            output_unknown[a] = unknown;
            output_values[a] = value;
        }
    }

    di_terminal_send(&self->output, simulation);
}

void di_and_init(DiAnd *self, size_t bits, size_t input_count) {
    di_element_init(&self->element);

    self->element.changed = di_and_changed;

    self->bits = bits;

    assert(input_count >= 2);

    di_gate_inputs_init(&self->inputs, input_count);

    for (size_t a = 0; a < input_count; a++) {
        di_terminal_init(di_gate_inputs_get(&self->inputs, a), &self->element, bits);
    }

    di_terminal_init(&self->output, &self->element, bits);
}

void di_and_destroy(DiAnd *self) {
    for (size_t a = 0; a < self->inputs.count; a++) {
        di_terminal_destroy(di_gate_inputs_get(&self->inputs, a));
    }

    di_gate_inputs_destroy(&self->inputs);

    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
