#include <digisim/elements/demultiplexer.h>

#include <assert.h>
#include <stdlib.h>

void di_demultiplexer_changed(DiElement *element, DiSimulation *simulation) {
    DiDemultiplexer *demultiplexer = (DiDemultiplexer *)element;

    DiSignal *select_signal = di_terminal_read(&demultiplexer->select);
    DiSignal *input_signal = di_terminal_read(&demultiplexer->input);

    assert(select_signal->bits < 20);

    uint64_t *values = di_signal_get_values(select_signal);
    uint64_t *error = di_signal_get_error(select_signal);
    uint64_t *unknown = di_signal_get_unknown(select_signal);
    uint64_t select = values[0];

    if (error[0] != 0 || unknown[0] != 0) {
        for (size_t a = 0; a < demultiplexer->output_count; a++) {
            di_terminal_fill(&demultiplexer->outputs[a], DI_BIT_ERROR, simulation);
        }

        return;
    }

    for (size_t a = 0; a < demultiplexer->output_count; a++) {
        if (a == select) {
            di_terminal_write(&demultiplexer->outputs[a], input_signal, simulation);
        } else {
            di_terminal_fill(&demultiplexer->outputs[a], DI_BIT_LOW, simulation);
        }
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