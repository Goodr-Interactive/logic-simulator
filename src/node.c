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
    node->hold = NULL;

    DiTerminal **values = di_node_connections_values(&node->connections);

    size_t bits = 0;

    bool failed = false;
    DiTerminal *hold = NULL;

    for (size_t a = 0; a < node->connections.count; a++) {
        if (bits == 0) {
            bits = values[a]->bits;
        }

        assert(values[a]->bits == bits);

        if (values[a]->holding) {
            if (hold) {
                failed = true;
                break;
            } else {
                hold = values[a];
            }
        }
    }

    // Becomes error, clear signal and re-simulate.
    if (failed) {
        if (node->has_signal) {
            di_signal_destroy(&node->signal);
        }

        // Changed.
        if (!node->error) {
            di_simulation_add(simulation, node);
        }

        node->error = true;
        node->hold = NULL;
        node->has_signal = false;

        return;
    }

    // assert !failed

    node->hold = hold;
    node->error = false;

    bool re_simulate = false;

    if (hold) {
        // By selection.
        assert(hold->holding);

        re_simulate = !node->has_signal;

        if (node->has_signal) {
            re_simulate = re_simulate || !di_signal_equal(&node->signal, &hold->signal);

            di_signal_destroy(&node->signal);
        }

        node->has_signal = true;
        di_signal_init_from(&node->signal, &hold->signal);
    } else {
        if (node->has_signal) {
            di_signal_destroy(&node->signal);

            re_simulate = true;
        }

        node->has_signal = false;
    }

    if (re_simulate) {
        di_simulation_add(simulation, node);
    }
}

void di_node_propagate(DiNode *node, DiSimulation *simulation) {
    DiTerminal **values = di_node_connections_values(&node->connections);

    for (size_t a = 0; a < node->connections.count; a++) {
        // Ignore holding connection (we don't need to send it back!)
        if (values[a] == node->hold) {
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

void di_node_connections_destroy(DiNodeConnections *list) {
    DiTerminal **values = di_node_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        values[a]->node = NULL;
    }

    if (list->heap_alloc) {
        free(list->heap);
    }
}

void di_node_init(DiNode *node) {
    node->hold = NULL;

    memset(&node->signal, 0, sizeof(DiSignal));
    di_node_connections_init(&node->connections);
}

void di_node_destroy(DiNode *node) { di_node_connections_destroy(&node->connections); }

void di_connect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    assert(!connection->node || connection->node == node);

    connection->node = node;
    di_node_connections_add(&node->connections, connection);

    di_node_changed(node, simulation);
}

void di_disconnect_simulate(DiNode *node, DiTerminal *connection, DiSimulation *simulation) {
    connection->node = NULL;
    di_node_connections_remove(&node->connections, connection);

    di_node_changed(node, simulation);
}

void di_connect(DiNode *node, DiTerminal *connection) { di_connect_simulate(node, connection, NULL); }

void di_disconnect(DiNode *node, DiTerminal *connection) { di_disconnect_simulate(node, connection, NULL); }
