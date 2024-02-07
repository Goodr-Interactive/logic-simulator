#include <digisim/elements/multiplexer.h>

#include <assert.h>
#include <stdlib.h>

void di_multiplexer_changed(DiElement *element, DiSimulation *simulation) {
    DiMultiplexer *multiplexer = (DiMultiplexer *)element;

    DiSignal *select_signal = di_terminal_read(&multiplexer->select);

    if (!select_signal) {
        di_terminal_fill(&multiplexer->output, DI_BIT_ERROR, simulation);

        return;
    }

    assert(select_signal->bits < 64);

    uint64_t *values = di_signal_values(select_signal);
    uint64_t select = values[0];

    if (select >= multiplexer->input_count) {
        di_terminal_fill(&multiplexer->output, DI_BIT_ERROR, simulation);

        return;
    }

    DiTerminal *input_terminal = &multiplexer->inputs[select];

    DiSignal *signal = di_terminal_read(input_terminal);

    if (!signal) {
        di_terminal_fill(&multiplexer->output, DI_BIT_ERROR, simulation);

        return;
    }

    DiSignal output;
    di_signal_init_from(&output, signal);

    di_terminal_write(&multiplexer->output, output, simulation);
}

void di_multiplexer_init(DiMultiplexer *multiplexer, size_t data_bits, size_t select_bits) {
    di_element_init(&multiplexer->element);

    multiplexer->element.changed = di_multiplexer_changed;

    assert(select_bits < 62); // still huge

    multiplexer->select_bits = select_bits;
    multiplexer->data_bits = data_bits;
    multiplexer->input_count = 1 << select_bits;

    multiplexer->inputs = malloc(sizeof(DiTerminal) * multiplexer->input_count);

    for (size_t a = 0; a < multiplexer->input_count; a++) {
        di_terminal_init(&multiplexer->inputs[a], &multiplexer->element, data_bits);
    }

    di_terminal_init(&multiplexer->select, &multiplexer->element, select_bits);
    di_terminal_init(&multiplexer->output, &multiplexer->element, data_bits);
}

void di_multiplexer_destroy(DiMultiplexer *multiplexer) {
    for (size_t a = 0; a < multiplexer->input_count; a++) {
        di_terminal_destroy(&multiplexer->inputs[a]);
    }

    di_terminal_destroy(&multiplexer->select);
    di_terminal_destroy(&multiplexer->output);

    free(multiplexer->inputs);

    di_element_destroy(&multiplexer->element);
}
