#include <digisim/node.h>

#include <digisim/terminal.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DiNodeConnection *di_wire_connections_values(DiNodeConnections *list) {
    if (list->heap_alloc) {
        return list->heap;
    } else {
        return list->local;
    }
}

void di_wire_connections_alloc(DiNodeConnections *list, size_t new_capacity) {
    list->capacity = new_capacity;

    if (list->heap_alloc) {
        list->heap =
            realloc(list->heap, list->capacity * sizeof(DiNodeConnection));
    } else {
        list->heap_alloc = true;

        DiNodeConnection *connections =
            malloc(list->capacity * sizeof(DiNodeConnection));
        memcpy(connections, list->local,
               list->count * sizeof(DiNodeConnection));

        list->heap = connections;
    }
}

bool di_wire_connections_add(DiNodeConnections *list, DiTerminal *value) {
    if (list->count >= list->capacity) {
        size_t new_capacity = 2 * list->capacity;

        if (new_capacity < list->count + 1) {
            new_capacity = list->count + 1;
        }

        di_wire_connections_alloc(list, new_capacity);
    }

    DiNodeConnection *values = di_wire_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a].terminal == value) {
            return false;
        }
    }

    assert(list->count < list->capacity);

    DiNodeConnection connection = {
        .terminal = value,
        .holding = false,
    };

    values[list->count] = connection;
    list->count++;

    return true;
}

bool di_wire_connections_remove(DiNodeConnections *list, DiTerminal *value) {
    size_t index = 0;

    DiNodeConnection *values = di_wire_connections_values(list);

    for (; index < list->count; index++) {
        if (values[index].terminal == value) {
            break;
        }
    }

    if (index >= list->count) {
        return false;
    }

    if (values[index].holding) {
        di_signal_destroy(&values[index].signal);
    }

    for (; index < list->count; index++) {
        values[index - 1] = values[index];
    }

    list->count--;

    return true;
}

// Consider return value as temporary.
DiNodeConnection *di_wire_connections_find(DiNodeConnections *list,
                                           DiTerminal *connection) {
    DiNodeConnection *values = di_wire_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a].terminal == connection) {
            return &values[a];
        }
    }

    return NULL;
}

void di_wire_connections_init(DiNodeConnections *list) {
    list->count = 0;
    list->capacity = DI_NODE_CONNECTIONS_SMALL_SIZE;

    list->heap_alloc = false;
}

void di_wire_connections_destroy(DiNodeConnections *list) {
    DiNodeConnection *values = di_wire_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        values[a].terminal->node = NULL;

        if (values[a].holding) {
            di_signal_destroy(&values[a].signal);
        }
    }

    if (list->heap_alloc) {
        free(list->heap);
    }
}

void di_node_init(DiNode *wire) {
    wire->hold = NULL;

    di_wire_connections_init(&wire->connections);
}

void di_node_destroy(DiNode *wire) {
    di_wire_connections_destroy(&wire->connections);
}

void di_wire_changed(DiNode *wire) {
    wire->hold = NULL;

    DiNodeConnection *values = di_wire_connections_values(&wire->connections);

    size_t bits = 0;

    bool failed = false;

    for (size_t a = 0; a < wire->connections.count; a++) {
        if (bits == 0) {
            bits = values[a].terminal->bits;
        }

        assert(values[a].terminal->bits == bits);

        if (!failed && values[a].holding) {
            if (wire->hold) {
                failed = true;
            } else {
                wire->hold = &values[a];
            }
        }
    }
}

void di_node_set(DiNode *wire, DiTerminal *source, DiSignal move_signal) {
    DiNodeConnection *connection =
        di_wire_connections_find(&wire->connections, source);

    if (!connection) {
        return;
    }

    connection->holding = true;
    connection->signal = move_signal;

    di_wire_changed(wire);
}

void di_node_reset(DiNode *wire, DiTerminal *source) {
    DiNodeConnection *connection =
        di_wire_connections_find(&wire->connections, source);

    if (!connection) {
        return;
    }

    if (connection->holding) {
        di_signal_destroy(&connection->signal);
    }

    connection->holding = false;

    di_wire_changed(wire);
}

void di_connect(DiNode *wire, DiTerminal *connection) {
    di_wire_connections_add(&wire->connections, connection);

    // Super important, hold can be freed by adding a connection.
    di_wire_changed(wire);
}

void di_disconnect(DiNode *wire, DiTerminal *connection) {
    di_node_reset(wire, connection);

    connection->node = NULL;
    di_wire_connections_remove(&wire->connections, connection);

    di_wire_changed(wire);
}
