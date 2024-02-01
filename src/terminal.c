#include <digisim/terminal.h>

#include <digisim/node.h>

void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits) {
    terminal->parent = parent;
    terminal->bits = bits;
    terminal->node = NULL;
}

void di_terminal_write(DiTerminal *terminal, DiSignal move_signal) {
    if (!terminal->node) {
        return;
    }

    di_node_set(terminal->node, terminal, move_signal);
}

void di_terminal_reset(DiTerminal *terminal) {
    if (!terminal->node) {
        return;
    }

    di_node_reset(terminal->node, terminal);
}

DiSignal *di_terminal_read(DiTerminal *terminal) {
    if (!terminal->node)
        return NULL;

    DiNodeConnection *wire = terminal->node->hold;

    if (!wire || !wire->holding) {
        return NULL;
    }

    return &wire->signal;
}
