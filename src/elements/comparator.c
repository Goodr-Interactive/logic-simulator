#include <digisim/elements/comparator.h>

#include <digisim/utility/signed.h>

void di_comparator_changed(DiElement *element, DiSimulation *simulation) {
    DiComparator *self = (DiComparator *)element;

    uint64_t value_a = di_signal_get_values(di_terminal_read(&self->input_a))[0] & DI_SIGNED_TOP_MASK(self->bits);
    uint64_t value_b = di_signal_get_values(di_terminal_read(&self->input_b))[0] & DI_SIGNED_TOP_MASK(self->bits);

    bool greater;
    bool equal;
    bool less;

    if (self->is_signed) {
        int64_t signed_a = di_signed_make(value_a, self->bits);
        int64_t signed_b = di_signed_make(value_b, self->bits);

        greater = signed_a > signed_b;
        equal = signed_a == signed_b;
        less = signed_a < signed_b;
    } else {
        greater = value_a > value_b;
        equal = value_a == value_b;
        less = value_a < value_b;
    }

    di_signal_set(&self->greater.signal, 0, greater);
    di_signal_set(&self->equal.signal, 0, equal);
    di_signal_set(&self->less.signal, 0, less);

    di_terminal_send(&self->greater, simulation);
    di_terminal_send(&self->equal, simulation);
    di_terminal_send(&self->less, simulation);
}

void di_comparator_init(DiComparator *self, size_t bits, bool is_signed) {
    di_element_init(&self->element);

    self->element.changed = di_comparator_changed;

    self->bits = bits;
    self->is_signed = is_signed;

    di_terminal_init(&self->input_a, &self->element, bits);
    di_terminal_init(&self->input_b, &self->element, bits);

    di_terminal_init(&self->greater, &self->element, 1);
    di_terminal_init(&self->equal, &self->element, 1);
    di_terminal_init(&self->less, &self->element, 1);
}

void di_comparator_destroy(DiComparator *self) {
    di_terminal_destroy(&self->input_a);
    di_terminal_destroy(&self->input_b);

    di_terminal_destroy(&self->greater);
    di_terminal_destroy(&self->equal);
    di_terminal_destroy(&self->less);

    di_element_destroy(&self->element);
}
