#include <digisim/elements/gate.h>

#include <digisim/node.h>

#include <assert.h>

void di_gate_changed(DiElement *component, DiSimulation *simulation) {
    DiGate *self = (DiGate *)component;

    DiSignal *output = &self->output.signal;

    di_signal_copy(output, di_terminal_read(di_gate_inputs_get(&self->inputs, 0)));

    uint64_t *output_values = di_signal_get_values(output);
    uint64_t *output_error = di_signal_get_error(output);
    uint64_t *output_unknown = di_signal_get_unknown(output);

    for (size_t a = 0; a < DI_SIGNAL_U64_COUNT(self->bits); a++) {
        uint64_t has_multiple = 0;

        for (size_t i = 1; i < self->inputs.count; i++) {
            DiSignal *input = di_terminal_read(di_gate_inputs_get(&self->inputs, i));

            uint64_t *in_values = di_signal_get_values(input);
            uint64_t *in_error = di_signal_get_error(input);
            uint64_t *in_unknown = di_signal_get_unknown(input);

            size_t error = output_error[a] | in_error[a];
            size_t unknown = output_unknown[a] & in_unknown[a];
            size_t value;

            switch (self->op) {
            case DI_GATE_OP_AND:
            case DI_GATE_OP_NAND:
                value = (output_values[a] & in_values[a]);
                break;
            case DI_GATE_OP_OR:
            case DI_GATE_OP_NOR:
                value = (output_values[a] | in_values[a]);
                break;
            case DI_GATE_OP_XOR:
            case DI_GATE_OP_XNOR:
                value = (output_values[a] ^ in_values[a]);
                break;
            case DI_GATE_OP_XOR_ANY:
            case DI_GATE_OP_XNOR_ANY:
                has_multiple = has_multiple | (output_values[a] & in_values[a]);
                value = ((output_values[a] | in_values[a]) & ~has_multiple);
                break;
            }

            output_error[a] = error;
            output_unknown[a] = unknown;
            output_values[a] = value;
        }

        switch (self->op) {
        case DI_GATE_OP_NAND:
        case DI_GATE_OP_NOR:
        case DI_GATE_OP_XNOR:
        case DI_GATE_OP_XNOR_ANY:
            output_values[a] = ~output_values[a];
        default:
            break;
        }

        output_values[a] = output_values[a] & ~output_error[a];
    }

    di_terminal_send(&self->output, simulation);
}

void di_gate_init(DiGate *self, DiGateOp op, size_t bits, size_t input_count) {
    di_element_init(&self->element);

    self->element.changed = di_gate_changed;

    self->op = op;
    self->bits = bits;

    assert(input_count >= 2);

    di_gate_inputs_init(&self->inputs, input_count);

    for (size_t a = 0; a < input_count; a++) {
        di_terminal_init(di_gate_inputs_get(&self->inputs, a), &self->element, bits);
    }

    di_terminal_init(&self->output, &self->element, bits);
}

void di_gate_destroy(DiGate *self) {
    for (size_t a = 0; a < self->inputs.count; a++) {
        di_terminal_destroy(di_gate_inputs_get(&self->inputs, a));
    }

    di_gate_inputs_destroy(&self->inputs);

    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
