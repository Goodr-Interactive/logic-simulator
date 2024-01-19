#pragma once

#include <digisim/signal.h>

#include <stddef.h>

typedef struct di_component_t DiComponent;
typedef struct di_wire_t DiWire;

typedef struct di_connection_t {
    DiComponent *parent;
    size_t bits;

    DiWire *wire;
} DiConnection;

void di_connection_init(DiConnection *connection, DiComponent *parent, size_t bits);
// No destroy.

void di_connection_write(DiConnection *connection, DiSignal move_signal);
void di_connection_reset(DiConnection *connection);

DiSignal *di_connection_read(DiConnection *connection);
