#include <digisim/elements/arithmetic.h>

#include <digisim/utility/signed.h>

#include <assert.h>

void di_arithmetic_changed(DiElement *element, DiSimulation *simulation) {
    DiArithmetic *self = (DiArithmetic *)element;

    DiSignal *in_a = di_terminal_read(&self->in_a);
    DiSignal *in_b = di_terminal_read(&self->in_b);

    uint64_t value_a = di_signal_get_values(in_a)[0] & ~DI_SIGNED_TOP_MASK(self->bits);
    uint64_t value_b = di_signal_get_values(in_b)[0] & ~DI_SIGNED_TOP_MASK(self->bits);

    uint64_t value;

    switch (self->op) {
    case DI_ARITHMETIC_OP_ADD:
        value = value_a + value_b;

        break;

    case DI_ARITHMETIC_OP_SUB:
        value = value_a - value_b;

        break;

    case DI_ARITHMETIC_OP_MUL:
        value = value_a * value_b;

        break;

    case DI_ARITHMETIC_OP_MUL_SIGNED:
        value = (uint64_t)(di_signed_make(value_a, self->bits) * di_signed_make(value_b, self->bits));

        break;
    }

    // Make sure we clear the output, e.g. set unknown and error bits to zero.
    di_signal_fill(&self->output.signal, DI_BIT_LOW);

    di_signal_get_values(&self->output.signal)[0] = value;

    di_terminal_send(&self->output, simulation);
}

void di_arithmetic_init(DiArithmetic *self, size_t bits, DiArithmeticOp op) {
    assert(bits <= 64);

    di_element_init(&self->element);

    self->element.changed = di_arithmetic_changed;

    self->bits = bits;
    self->op = op;

    di_terminal_init(&self->in_a, &self->element, bits);
    di_terminal_init(&self->in_b, &self->element, bits);

    di_terminal_init(&self->output, &self->element, bits);
}

void di_arithmetic_destroy(DiArithmetic *self) {
    di_terminal_destroy(&self->in_a);
    di_terminal_destroy(&self->in_b);

    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
