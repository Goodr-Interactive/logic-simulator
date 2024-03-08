#include <digisim/elements/register.h>

#include <stdlib.h>
#include <string.h>

void di_signal_copy_into_list(DiSignal *signal, bool *state) {
    for (size_t a = 0; a < signal->bits; a++) {
        state[a] = di_signal_get(signal, a) == DI_BIT_HIGH;
    }
}

void di_signal_copy_from_list(DiSignal *signal, bool *state) {
    for (size_t a = 0; a < signal->bits; a++) {
        di_signal_set(signal, a, state[a]);
    }
}

void di_register_changed(DiElement *element, DiSimulation *simulation) {
    DiRegister *reg = (DiRegister *)element;

    DiSignal *clock = di_terminal_read(&reg->clock);
    DiSignal *reset = di_terminal_read(&reg->reset);
    DiSignal *data = di_terminal_read(&reg->data);

    if (di_signal_get(reset, 0) == DI_BIT_HIGH) {
        reg->did_send = false;

        memset(reg->hold, 0, reg->bits * sizeof(bool));
        memset(reg->state, 0, reg->bits * sizeof(bool));

        di_signal_fill(&reg->value.signal, DI_BIT_LOW);

        di_terminal_send(&reg->value, simulation);
    }

    if (di_signal_get(clock, 0) == DI_BIT_HIGH) {
        if (!reg->did_send) {
            reg->did_send = true;

            // Is the double buffer really more than we need?
            memcpy(reg->state, reg->hold, reg->bits * sizeof(bool));
            di_signal_copy_from_list(&reg->value.signal, reg->state);

            di_terminal_send(&reg->value, simulation);
        }
    } else {
        reg->did_send = false;

        di_signal_copy_into_list(data, reg->hold);
    }
}

void di_register_reset(DiElement *element) {
    DiRegister *reg = (DiRegister *)element;

    reg->did_send = false;

    memset(reg->hold, 0, reg->bits * sizeof(bool));
    memset(reg->state, 0, reg->bits * sizeof(bool));
}

void di_register_init(DiRegister *reg, size_t bits) {
    di_element_init(&reg->element);

    reg->element.changed = di_register_changed;

    reg->bits = bits;
    reg->did_send = false;
    reg->hold = malloc(bits * sizeof(bool));
    reg->state = malloc(bits * sizeof(bool));

    di_terminal_init(&reg->data, &reg->element, bits);
    di_terminal_init(&reg->clock, &reg->element, 1);
    di_terminal_init(&reg->reset, &reg->element, 1);
    di_terminal_init(&reg->value, &reg->element, bits);
}

void di_register_destroy(DiRegister *reg) {
    free(reg->hold);
    free(reg->state);

    di_element_destroy(&reg->element);

    di_terminal_destroy(&reg->data);
    di_terminal_destroy(&reg->clock);
    di_terminal_destroy(&reg->value);
}
