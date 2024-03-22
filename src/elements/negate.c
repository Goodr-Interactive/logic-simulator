#include <digisim/elements/negate.h>

#include <assert.h>

#define DI_NEGATE_TOP_MASK(bits) ((~(uint64_t)0) << (bits))

int64_t di_negate_sign(uint64_t value, size_t bits) {
    uint64_t mask = 1 << (bits - 1);

    if ((mask & value) != 0) {
        // Two's complement casting.
        // Set the sign bits for the native system to interpret this properly.
        return (int64_t)(value | DI_NEGATE_TOP_MASK(bits));
    } else {
        return (int64_t)value;
    }
}


void di_negate_changed(DiElement *element, DiSimulation *simulation) {
    DiNegate *negate = (DiNegate *)element;

    uint64_t value = di_signal_get_values(di_terminal_read(&negate->input))[0];
    int64_t negated = -di_negate_sign(value, negate->bits);

    di_signal_get_values(&negate->output.signal)[0] = negated;

    di_terminal_send(&negate->output, simulation);
}

void di_negate_init(DiNegate *negate, size_t bits) {
    assert(bits <= 64);

    di_element_init(&negate->element);

    negate->element.changed = di_negate_changed;

    di_terminal_init(&negate->input, &negate->element, bits);
    di_terminal_init(&negate->output, &negate->element, bits);
}

void di_negate_destroy(DiNegate *negate) {
    di_terminal_destroy(&negate->input);
    di_terminal_destroy(&negate->output);

    di_element_destroy(&negate->element);
}
