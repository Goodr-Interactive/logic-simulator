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

    for (size_t a = 0; a < node->connections.count; a++) {
        DiTerminal *terminal = values[a];

        if (!terminal->holding) {
            continue;
        }

        di_signal_merge(&node->signal, &node->signal, &terminal->signal);
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

    di_node_changed(node, simulation);
}

void di_disconnect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    connection->node = NULL;

    di_terminal_list_remove(&node->connections, connection);

    di_node_changed(node, simulation);
}

void di_connect(DiNode *node, DiTerminal *connection) { di_connect_simulate(node, connection, NULL); }

void di_disconnect(DiNode *node, DiTerminal *connection) { di_disconnect_simulate(node, connection, NULL); }
