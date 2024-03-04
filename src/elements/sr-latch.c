#include <digisim/elements/sr-latch.h>

#include <stdlib.h>

void di_sr_latch_changed(DiElement *element, DiSimulation *simulation) {
    DiSrLatch *self = (DiSrLatch *)element;

    DiSignal *in_set = di_terminal_read(&self->set);
    DiSignal *in_reset = di_terminal_read(&self->reset);

    DiSignal *output = &self->value.signal;

    for (size_t a = 0; a < self->bits; a++) {
        bool set = di_bit_value(di_signal_get(in_set, a), false);
        bool reset = di_bit_value(di_signal_get(in_reset, a), false);

        if (set) {
            self->state[a] = true;
        }

        if (reset) {
            self->state[a] = false;
        }

        di_signal_set(output, a, self->state[a]);
    }

    di_terminal_send(&self->value, simulation);
}

void di_sr_latch_init(DiSrLatch *latch, size_t bits) {
    di_element_init(&latch->element);

    latch->element.changed = di_sr_latch_changed;

    latch->bits = bits;
    latch->state = malloc(bits * sizeof(bool));

    di_terminal_init(&latch->set, &latch->element, bits);
    di_terminal_init(&latch->reset, &latch->element, bits);
    di_terminal_init(&latch->value, &latch->element, bits);
}

void di_sr_latch_destroy(DiSrLatch *latch) {
    di_element_destroy(&latch->element);

    di_terminal_destroy(&latch->set);
    di_terminal_destroy(&latch->reset);
    di_terminal_destroy(&latch->value);
}
