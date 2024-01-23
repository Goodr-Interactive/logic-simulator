#pragma once

#include <digisim/signal.h>

#include <stddef.h>

typedef struct di_element_t DiElement;
typedef struct di_node_t DiNode;

typedef struct di_terminal_t {
    DiElement *parent;
    size_t bits;

    DiNode *node;
} DiTerminal;

void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits);
// No destroy.

void di_terminal_write(DiTerminal *terminal, DiSignal move_signal);
void di_terminal_reset(DiTerminal *terminal);

DiSignal *di_terminal_read(DiTerminal *terminal);
