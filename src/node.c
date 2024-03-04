#include <digisim/node.h>

#include <digisim/element.h>
#include <digisim/simulation.h>
#include <digisim/terminal.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DiTerminal **di_node_connections_values(DiNodeConnections *list) {
    if (list->heap_alloc) {
        return list->heap;
    } else {
        return list->local;
    }
}

void di_node_connections_alloc(DiNodeConnections *list, size_t new_capacity) {
    list->capacity = new_capacity;

    if (list->heap_alloc) {
        list->heap = realloc(list->heap, list->capacity * sizeof(DiTerminal *));
    } else {
        list->heap_alloc = true;

        DiTerminal **connections = malloc(list->capacity * sizeof(DiTerminal *));
        memcpy(connections, list->local, list->count * sizeof(DiTerminal *));

        list->heap = connections;
    }
}

void di_node_changed(DiNode *node, DiSimulation *simulation) {
    DiTerminal **values = di_node_connections_values(&node->connections);

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
    DiTerminal **values = di_node_connections_values(&node->connections);

    for (size_t a = 0; a < node->connections.count; a++) {
        // Ignore holding connection (we don't need to send it back!)
        if (values[a]->holding) {
            continue;
        }

        di_element_changed(values[a]->parent, simulation);
    }
}

bool di_node_connections_add(DiNodeConnections *list, DiTerminal *value) {
    if (list->count >= list->capacity) {
        size_t new_capacity = 2 * list->capacity;

        if (new_capacity < list->count + 1) {
            new_capacity = list->count + 1;
        }

        di_node_connections_alloc(list, new_capacity);
    }

    DiTerminal **values = di_node_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a] == value) {
            return false;
        }
    }

    assert(list->count < list->capacity);

    values[list->count] = value;
    list->count++;

    return true;
}

bool di_node_connections_remove(DiNodeConnections *list, DiTerminal *value) {
    size_t index = 0;

    DiTerminal **values = di_node_connections_values(list);

    for (; index < list->count; index++) {
        if (values[index] == value) {
            break;
        }
    }

    if (index >= list->count) {
        return false;
    }

    for (; index < list->count; index++) {
        values[index - 1] = values[index];
    }

    list->count--;

    return true;
}

void di_node_connections_init(DiNodeConnections *list) {
    list->count = 0;
    list->capacity = DI_NODE_CONNECTIONS_SMALL_SIZE;

    list->heap_alloc = false;
}

void di_node_connections_destroy(DiNodeConnections *list, DiNode *node) {
    DiTerminal **values = di_node_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (node) {
            di_node_list_remove(&values[a]->parent->connections, node);
        }

        values[a]->node = NULL;
    }

    if (list->heap_alloc) {
        free(list->heap);
    }
}

void di_node_init(DiNode *node, size_t bits) {
    node->bits = bits;

    node->signal = di_signal_filled(bits, DI_BIT_UNKNOWN);
    node->last_signal = di_signal_filled(bits, DI_BIT_UNKNOWN);

    di_node_connections_init(&node->connections);
}

void di_node_destroy(DiNode *node) {
    di_signal_destroy(&node->signal);
    di_signal_destroy(&node->last_signal);

    di_node_connections_destroy(&node->connections, node);
}

void di_connect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    assert(!connection->node || connection->node == node);
    assert(connection->bits == node->bits);

    connection->node = node;
    di_node_connections_add(&node->connections, connection);
    di_node_list_add(&connection->parent->connections, node);

    di_node_changed(node, simulation);
}

void di_disconnect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    connection->node = NULL;

    di_node_connections_remove(&node->connections, connection);
    di_node_list_remove(&connection->parent->connections, node);

    di_node_changed(node, simulation);
}

void di_connect(DiNode *node, DiTerminal *connection) { di_connect_simulate(node, connection, NULL); }

void di_disconnect(DiNode *node, DiTerminal *connection) { di_disconnect_simulate(node, connection, NULL); }
