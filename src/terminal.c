#include <digisim/terminal.h>

#include <digisim/node.h>
#include <digisim/element.h>

#include <assert.h>
#include <string.h>

void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits) {
    terminal->parent = parent;
    terminal->bits = bits;
    terminal->node = NULL;

    terminal->holding = false;
    di_signal_init(&terminal->signal, bits);

    di_terminal_list_add(&parent->connections, terminal);
}

void di_terminal_destroy(DiTerminal *terminal) {
    // Disconnect all wires first, please!
    assert(!terminal->node);

    di_signal_destroy(&terminal->signal);
}

void di_terminal_write(DiTerminal *terminal, DiSignal *signal, DiSimulation *simulation) {
    assert(signal->bits == terminal->bits);

    di_signal_copy(&terminal->signal, signal);

    di_terminal_send(terminal, simulation);
}

void di_terminal_send(DiTerminal *terminal, DiSimulation *simulation) {
    terminal->holding = true;

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

void di_terminal_fill(DiTerminal *terminal, DiBit bit, DiSimulation *simulation) {
    di_signal_fill(&terminal->signal, bit);

    di_terminal_send(terminal, simulation);
}

DiSignal *di_terminal_read(DiTerminal *terminal) {
    // Return the value of the connected wire if connected.
    if (terminal->node) {
        return &terminal->node->signal;
    }

    // Otherwise, we will return the local signal value.
    // If the terminal is being held we can't modify our internal signal value, so we can just return it.
    // Otherwise, we should initialize it to something consistent DI_BIT_UNKNOWN.
    if (!terminal->holding) {
        di_signal_fill(&terminal->signal, DI_BIT_UNKNOWN);
    }

    return &terminal->signal;
}
