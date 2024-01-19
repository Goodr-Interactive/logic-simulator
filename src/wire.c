#include <digisim/wire.h>

#include <digisim/connection.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DiWireConnection *di_wire_connections_values(DiWireConnections *list) {
    if (list->heap_alloc) {
        return list->heap;
    } else {
        return list->local;
    }
}

void di_wire_connections_alloc(DiWireConnections *list, size_t new_capacity) {
    list->capacity = new_capacity;

    if (list->heap_alloc) {
        list->heap = realloc(list->heap, list->capacity * sizeof(DiWireConnection));
    } else {
        list->heap_alloc = true;

        DiWireConnection *connections = malloc(list->capacity * sizeof(DiWireConnection));
        memcpy(connections, list->local, list->count * sizeof(DiWireConnection));

        list->heap = connections;
    }
}

bool di_wire_connections_add(DiWireConnections *list, DiConnection *value) {
    if (list->count >= list->capacity) {
        size_t new_capacity = 2 * list->capacity;

        if (new_capacity < list->count + 1) {
            new_capacity = list->count + 1;
        }

        di_wire_connections_alloc(list, new_capacity);
    }

    DiWireConnection *values = di_wire_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a].connection == value) {
            return false;
        }
    }

    assert(list->count < list->capacity);

    DiWireConnection connection = {
        .connection = value,
        .holding = false,
    };

    values[list->count] = connection;
    list->count++;

    return true;
}

bool di_wire_connections_remove(DiWireConnections *list, DiConnection *value) {
    size_t index = 0;

    DiWireConnection *values = di_wire_connections_values(list);

    for (; index < list->count; index++) {
        if (values[index].connection == value) {
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
DiWireConnection *di_wire_connections_find(DiWireConnections *list, DiConnection *connection) {
    DiWireConnection *values = di_wire_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a].connection == connection) {
            return &values[a];
        }
    }

    return NULL;
}

void di_wire_connections_init(DiWireConnections *list) {
    list->count = 0;
    list->capacity = DI_WIRE_CONNECTIONS_SMALL_SIZE;

    list->heap_alloc = false;
}

void di_wire_connections_destroy(DiWireConnections *list) {
    DiWireConnection *values = di_wire_connections_values(list);

    for (size_t a = 0; a < list->count; a++) {
        values[a].connection->wire = NULL;

        if (values[a].holding) {
            di_signal_destroy(&values[a].signal);
        }
    }

    if (list->heap_alloc) {
        free(list->heap);
    }
}

void di_wire_init(DiWire *wire) {
    wire->hold = NULL;

    di_wire_connections_init(&wire->connections);
}

void di_wire_destroy(DiWire *wire) {
    di_wire_connections_destroy(&wire->connections);
}

void di_wire_changed(DiWire *wire) {
    wire->hold = NULL;

    DiWireConnection *values = di_wire_connections_values(&wire->connections);

    size_t bits = 0;

    bool failed = false;

    for (size_t a = 0; a < wire->connections.count; a++) {
        if (bits == 0) {
            bits = values[a].connection->bits;
        }

        assert(values[a].connection->bits == bits);

        if (!failed && values[a].holding) {
            if (wire->hold) {
                failed = true;
            } else {
                wire->hold = &values[a];
            }
        }
    }
}

void di_wire_set(DiWire *wire, DiConnection *source, DiSignal move_signal) {
    DiWireConnection *connection = di_wire_connections_find(&wire->connections, source);

    if (!connection) {
        return;
    }

    connection->holding = true;
    connection->signal = move_signal;

    di_wire_changed(wire);
}

void di_wire_reset(DiWire *wire, DiConnection *source) {
    DiWireConnection *connection = di_wire_connections_find(&wire->connections, source);

    if (!connection) {
        return;
    }

    if (connection->holding) {
        di_signal_destroy(&connection->signal);
    }

    connection->holding = false;

    di_wire_changed(wire);
}

void di_connect(DiWire *wire, DiConnection *connection) {
    di_wire_connections_add(&wire->connections, connection);

    // Super important, hold can be freed by adding a connection.
    di_wire_changed(wire);
}

void di_disconnect(DiWire *wire, DiConnection *connection) {
    di_wire_reset(wire, connection);

    connection->wire = NULL;
    di_wire_connections_remove(&wire->connections, connection);

    di_wire_changed(wire);
}
