#include <digisim/elements/d-latch.h>

#include <stdlib.h>

void di_d_latch_changed(DiElement *element, DiSimulation *simulation) {
    DiDLatch *self = (DiDLatch *)element;

    DiSignal *clock = di_terminal_read(&self->clock);
    DiSignal *data = di_terminal_read(&self->data);

    bool transparent = di_signal_get(clock, 1);

    if (transparent) {
        for (size_t a = 0; a < self->bits; a++) {
            self->state[a] = di_signal_get(data, a) == DI_BIT_HIGH;
        }
    }

    for (size_t a = 0; a < self->bits; a++) {
        di_signal_set(&self->value.signal, a, self->state[a]);
    }

    di_terminal_send(&self->value, simulation);
}

void di_d_latch_init(DiDLatch *latch, size_t bits) {
    di_element_init(&latch->element);

    latch->element.changed = di_d_latch_changed;

    latch->bits = bits;
    latch->state = malloc(bits * sizeof(bool));

    di_terminal_init(&latch->data, &latch->element, bits);
    di_terminal_init(&latch->clock, &latch->element, 1);
    di_terminal_init(&latch->value, &latch->element, bits);
}

void di_d_latch_destroy(DiDLatch *latch) {
    di_element_destroy(&latch->element);

    di_terminal_destroy(&latch->data);
    di_terminal_destroy(&latch->clock);
    di_terminal_destroy(&latch->value);
}
