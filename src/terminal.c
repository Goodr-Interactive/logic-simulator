#include <digisim/terminal.h>

#include <digisim/node.h>

#include <assert.h>
#include <string.h>

void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits) {
    terminal->parent = parent;
    terminal->bits = bits;
    terminal->node = NULL;

    terminal->holding = false;
    memset(&terminal->signal, 0, sizeof(DiSignal));
}

void di_terminal_destroy(DiTerminal *terminal) {
    // Disconnect all wires first, please!
    assert(!terminal->node);

    if (terminal->holding) {
        di_signal_destroy(&terminal->signal);
    }
}

void di_terminal_write(DiTerminal *terminal, DiSignal move_signal, DiSimulation *simulation) {
    if (terminal->holding) {
        di_signal_destroy(&terminal->signal);
    }

    terminal->holding = true;
    terminal->signal = move_signal;

    if (terminal->node) {
        di_node_changed(terminal->node, simulation);
    }
}

void di_terminal_reset(DiTerminal *terminal, DiSimulation *simulation) {
    if (terminal->holding) {
        di_signal_destroy(&terminal->signal);
    }

    terminal->holding = false;

    if (terminal->node) {
        di_node_changed(terminal->node, simulation);
    }
}

DiSignal *di_terminal_read(DiTerminal *terminal) {
    if (!terminal->node)
        return NULL;

    DiTerminal *wire = terminal->node->hold;

    if (!wire) {
        return NULL;
    }

    assert(wire->holding);

    return &wire->signal;
}
