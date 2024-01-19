#pragma once

#include <digisim/signal.h>

#include <stddef.h>
#include <stdbool.h>

#define DI_WIRE_CONNECTIONS_SMALL_SIZE 4

typedef struct di_connection_t DiConnection;

typedef struct di_wire_connection_t {
    DiConnection *connection;

    bool holding;
    DiSignal signal;
} DiWireConnection;

typedef struct di_wire_connections_t {
    size_t count;
    size_t capacity;

    bool heap_alloc;

    union {
        DiWireConnection local[DI_WIRE_CONNECTIONS_SMALL_SIZE];
        DiWireConnection *heap;
    };
} DiWireConnections;

typedef struct di_wire_t {
    DiWireConnections connections;

    DiWireConnection *hold;
} DiWire;

void di_wire_init(DiWire *wire);
void di_wire_destroy(DiWire *wire);

void di_wire_set(DiWire *wire, DiConnection *source, DiSignal move_signal);
void di_wire_reset(DiWire *wire, DiConnection *source);

void di_connect(DiWire *wire, DiConnection *connection);
void di_disconnect(DiWire *wire, DiConnection *connection);
