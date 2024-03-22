#ifndef DIGISIM_ELEMENTS_SPLITTER_H
#define DIGISIM_ELEMENTS_SPLITTER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_splitter_t {
    DiElement element;

    size_t bits;

    DiTerminal end;

    size_t split_count;
    DiTerminal *splits;

    DiSignal split_accumulator;
} DiSplitter;

void di_splitter_init(DiSplitter *splitter, size_t bits, size_t split_count, const size_t *splits);
void di_splitter_destroy(DiSplitter *splitter);

#endif // DIGISIM_ELEMENTS_SPLITTER_H
