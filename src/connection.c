#include <digisim/connection.h>

#include <digisim/wire.h>

void di_connection_init(DiConnection *connection, DiComponent *parent, size_t bits) {
    connection->parent = parent;
    connection->bits = bits;
    connection->wire = NULL;
}

void di_connection_write(DiConnection *connection, DiSignal move_signal) {
    if (!connection->wire) {
        return;
    }

    di_wire_set(connection->wire, connection, move_signal);
}

void di_connection_reset(DiConnection *connection) {
    if (!connection->wire) {
        return;
    }

    di_wire_reset(connection->wire, connection);
}

DiSignal *di_connection_read(DiConnection *connection) {
    if (!connection->wire)
        return NULL;

    DiWireConnection *wire = connection->wire->hold;

    if (!wire || !wire->holding) {
        return NULL;
    }

    return &wire->signal;
}
