#include <digisim/node.h>

#include <digisim/element.h>
#include <digisim/simulation.h>
#include <digisim/terminal.h>

#include <assert.h>
#include <string.h>

void di_node_changed(DiNode *node, DiSimulation *simulation) {
    DiTerminal **values = di_terminal_list_values(&node->connections);

    di_signal_copy(&node->last_signal, &node->signal);

    di_signal_fill(&node->signal, DI_BIT_UNKNOWN);

    uint64_t *signal_values = di_signal_get_values(&node->signal);
    uint64_t *signal_error = di_signal_get_error(&node->signal);
    uint64_t *signal_unknown = di_signal_get_unknown(&node->signal);

    for (size_t a = 0; a < node->connections.count; a++) {
        DiTerminal *terminal = values[a];

        if (!terminal->holding) {
            continue;
        }

        uint64_t *terminal_values = di_signal_get_values(&terminal->signal);
        uint64_t *terminal_error = di_signal_get_error(&terminal->signal);
        uint64_t *terminal_unknown = di_signal_get_unknown(&terminal->signal);

        for (size_t i = 0; i < DI_SIGNAL_U64_COUNT(node->bits); i++) {
            uint64_t error = signal_error[i] | terminal_error[i] |
                             (~(signal_unknown[i] | terminal_unknown[i]) & (signal_values[i] ^ terminal_values[i]));
            uint64_t unknown = signal_unknown[i] & terminal_unknown[i] & ~error;
            uint64_t value = signal_values[i] | terminal_values[i] & ~error;

            signal_values[i] = value;
            signal_error[i] = error;
            signal_unknown[i] = unknown;
        }
    }

    if (!di_signal_equal(&node->signal, &node->last_signal)) {
        di_simulation_add(simulation, node);
    }
}

void di_node_propagate(DiNode *node, DiSimulation *simulation) {
    DiTerminal **values = di_terminal_list_values(&node->connections);

    for (size_t a = 0; a < node->connections.count; a++) {
        // Ignore holding connection (we don't need to send it back!)
        if (values[a]->holding) {
            continue;
        }

        di_element_changed(values[a]->parent, simulation);
    }
}

void di_node_init(DiNode *node, size_t bits) {
    node->bits = bits;

    node->signal = di_signal_filled(bits, DI_BIT_UNKNOWN);
    node->last_signal = di_signal_filled(bits, DI_BIT_UNKNOWN);

    di_terminal_list_init(&node->connections);
}

void di_node_connections_destroy(DiNode *node) {
    DiTerminal **values = di_terminal_list_values(&node->connections);

    for (size_t a = 0; a < node->connections.count; a++) {
        values[a]->node = NULL;
    }

    di_terminal_list_destroy(&node->connections);
}

void di_node_destroy(DiNode *node) {
    di_signal_destroy(&node->signal);
    di_signal_destroy(&node->last_signal);

    di_node_connections_destroy(node);
}

void di_connect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    assert(!connection->node || connection->node == node);
    assert(connection->bits == node->bits);

    connection->node = node;
    di_terminal_list_add(&node->connections, connection);

    // Changing the wire signal value without a simulation can cause a change that will never be propagated.
    if (simulation) {
        di_node_changed(node, simulation);
    }
}

void di_disconnect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    connection->node = NULL;

    di_terminal_list_remove(&node->connections, connection);

    if (simulation) {
        di_node_changed(node, simulation);
    }
}

void di_connect(DiNode *node, DiTerminal *connection) { di_connect_simulate(node, connection, NULL); }

void di_disconnect(DiNode *node, DiTerminal *connection) { di_disconnect_simulate(node, connection, NULL); }
