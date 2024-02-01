#pragma once

#include <digisim/signal.h>

#include <stddef.h>

typedef struct di_element_t DiElement;
typedef struct di_node_t DiNode;
typedef struct di_simulation_t DiSimulation;

typedef struct di_terminal_t {
    DiElement *parent;
    size_t bits;

    DiNode *node;

    // For output.
    bool holding;
    DiSignal signal;
} DiTerminal;

void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits);
void di_terminal_destroy(DiTerminal *terminal);

void di_terminal_write(DiTerminal *terminal, DiSignal move_signal, DiSimulation *simulation);
void di_terminal_reset(DiTerminal *terminal, DiSimulation *simulation);

DiSignal *di_terminal_read(DiTerminal *terminal);
