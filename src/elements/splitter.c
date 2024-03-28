#include <digisim/elements/splitter.h>

#include <stdlib.h>
#include <assert.h>

void di_splitter_accumulate(DiSignal *out, size_t count, DiTerminal *terminals, DiSignal *signals) {
    size_t bit_count = 0;

    for (size_t a = 0; a < count; a++) {
        DiTerminal *terminal = &terminals[a];

        di_terminal_directional_read(terminal, &signals[a]);

        for (size_t b = 0; b < terminal->bits; b++) {
            di_signal_set(out, bit_count + b, di_signal_get(&signals[a], b));
        }

        bit_count += terminal->bits;
    }
}

void di_splitter_spread(DiSignal *in, size_t count, DiTerminal *terminals) {
    size_t bit_count = 0;

    for (size_t a = 0; a < count; a++) {
        DiTerminal *terminal = &terminals[a];

        for (size_t b = 0; b < terminal->bits; b++) {
            di_signal_set(&terminal->signal, b, di_signal_get(in, bit_count + b));
        }

        bit_count += terminal->bits;
    }
}

void di_splitter_changed(DiElement *element, DiSimulation *simulation) {
    DiSplitter *splitter = (DiSplitter *)element;

    // We have to first release our influence on the other wires!
    di_terminal_directional_read(&splitter->end, &splitter->end_signal);

    di_splitter_accumulate(&splitter->split_accumulator, splitter->split_count, splitter->splits, splitter->split_signals);
    di_signal_merge(&splitter->split_accumulator, &splitter->split_accumulator, &splitter->end_signal);

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
    di_signal_init(&splitter->end_signal, bits);

    splitter->split_signals = malloc(split_count * sizeof(DiSignal));

    size_t bits_accounted = 0;

    for (size_t a = 0; a < split_count; a++) {
        size_t split = splits[a];

        bits_accounted += split;

        assert(bits_accounted <= bits);

        di_terminal_init(&splitter->splits[a], &splitter->element, split);
        di_signal_init(&splitter->split_signals[a], split);
    }
}

void di_splitter_destroy(DiSplitter *splitter) {
    di_signal_destroy(&splitter->split_accumulator);
    di_signal_destroy(&splitter->end_signal);

    di_terminal_destroy(&splitter->end);

    for (size_t a = 0; a < splitter->split_count; a++) {
        di_terminal_destroy(&splitter->splits[a]);
        di_signal_destroy(&splitter->split_signals[a]);
    }

    free(splitter->split_signals);

    di_element_destroy(&splitter->element);
}
