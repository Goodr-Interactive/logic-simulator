#include <digisim/elements/splitter.h>

#include <stdlib.h>
#include <assert.h>

void di_splitter_accumulate(DiSignal *out, size_t count, DiTerminal *terminals) {
    size_t bit_count = 0;

    for (size_t a = 0; a < count; a++) {
        DiTerminal *terminal = &terminals[a];

        DiSignal *read = di_terminal_read(terminal);

        for (size_t b = 0; b < count; b++) {
            di_signal_set(out, bit_count + b, di_signal_get(read, b));
        }

        bit_count += terminal->bits;
    }
}

void di_splitter_spread(DiSignal *in, size_t count, DiTerminal *terminals) {
    size_t bit_count = 0;

    for (size_t a = 0; a < count; a++) {
        DiTerminal *terminal = &terminals[a];

        for (size_t b = 0; b < count; b++) {
            di_signal_set(&terminals->signal, b, di_signal_get(in, bit_count + b));
        }

        bit_count += terminal->bits;
    }
}

void di_splitter_changed(DiElement *element, DiSimulation *simulation) {
    DiSplitter *splitter = (DiSplitter *)element;

    DiSignal *end_value = di_terminal_read(&splitter->end);

    di_splitter_accumulate(&splitter->split_accumulator, splitter->split_count, splitter->splits);
    di_signal_merge(&splitter->split_accumulator, &splitter->split_accumulator, end_value);

    di_signal_copy(&splitter->end.signal, &splitter->split_accumulator);
    di_splitter_spread(&splitter->split_accumulator, splitter->split_count, splitter->splits);

    di_terminal_send(&splitter->end, simulation);

    for (size_t a = 0; a < splitter->split_count; a++) {
        di_terminal_send(&splitter->splits[a], simulation);
    }
}

void di_splitter_init(DiSplitter *splitter, size_t bits, size_t split_count, const size_t *splits) {
    di_element_init(&splitter->element);

    splitter->element.changed = di_splitter_changed;

    splitter->bits = bits;

    splitter->split_count = split_count;
    splitter->splits = malloc(split_count * sizeof(DiTerminal));

    di_terminal_init(&splitter->end, &splitter->element, bits);

    di_signal_init(&splitter->split_accumulator, bits);

    size_t bits_accounted = 0;

    for (size_t a = 0; a < split_count; a++) {
        size_t split = splits[a];

        bits_accounted += split;

        assert(bits_accounted < bits);

        di_terminal_init(&splitter->splits[a], &splitter->element, bits);
    }
}

void di_splitter_destroy(DiSplitter *splitter) {
    di_signal_destroy(&splitter->split_accumulator);

    di_terminal_destroy(&splitter->end);

    for (size_t a = 0; a < splitter->split_count; a++) {
        di_terminal_destroy(&splitter->splits[a]);
    }

    di_element_destroy(&splitter->element);
}
