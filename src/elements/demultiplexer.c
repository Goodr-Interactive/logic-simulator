#include <digisim/elements/demultiplexer.h>

#include <assert.h>
#include <stdlib.h>

void di_demultiplexer_disconnect(DiDemultiplexer *demultiplexer, DiSimulation *simulation) {
    for (size_t a = 0; a < demultiplexer->output_count; a++) {
        di_terminal_reset(&demultiplexer->outputs[a], simulation);
    }
}

void di_demultiplexer_changed(DiElement *element, DiSimulation *simulation) {
    DiDemultiplexer *demultiplexer = (DiDemultiplexer *)element;

    DiSignal *select_signal = di_terminal_read(&demultiplexer->select);

    if (!select_signal) {
        di_demultiplexer_disconnect(demultiplexer, simulation);

        return;
    }

    assert(select_signal->bits < 64);

    uint64_t *values = di_signal_values(select_signal);
    uint64_t select = values[0];

    for (size_t a = 0; a < demultiplexer->output_count; a++) {
        DiSignal output;

        if (a == select) {
            di_signal_copy(&output, select_signal);
        } else {
            di_signal_fill(&output, DI_BIT_LOW);
        }

        di_terminal_write(&demultiplexer->outputs[a], output, simulation);
    }
}

void di_demultiplexer_init(DiDemultiplexer *demultiplexer, size_t data_bits, size_t select_bits) {
    di_element_init(&demultiplexer->element);

    demultiplexer->element.changed = di_demultiplexer_changed;

    assert(select_bits < 62); // still huge

    demultiplexer->data_bits = data_bits;
    demultiplexer->select_bits = select_bits;
    demultiplexer->output_count = 1 << select_bits;

    demultiplexer->outputs = malloc(sizeof(DiTerminal) * demultiplexer->output_count);

    for (size_t a = 0; a < demultiplexer->output_count; a++) {
        di_terminal_init(&demultiplexer->outputs[a], &demultiplexer->element, data_bits);
    }

    di_terminal_init(&demultiplexer->input, &demultiplexer->element, select_bits);
    di_terminal_init(&demultiplexer->select, &demultiplexer->element, select_bits);
}

void di_demultiplexer_destroy(DiDemultiplexer *demultiplexer) {
    for (size_t a = 0; a < demultiplexer->output_count; a++) {
        di_terminal_destroy(&demultiplexer->outputs[a]);
    }

    free(demultiplexer->outputs);

    di_terminal_destroy(&demultiplexer->input);
    di_terminal_destroy(&demultiplexer->select);

    di_element_destroy(&demultiplexer->element);
}