#pragma once

#include <digisim/signal.h>

#include <stdbool.h>
#include <stddef.h>

#define DI_NODE_CONNECTIONS_SMALL_SIZE 4

typedef struct di_terminal_t DiTerminal;

typedef struct di_node_connection_t {
    DiTerminal *terminal;

    bool holding;
    DiSignal signal;
} DiNodeConnection;

typedef struct di_node_connections_t {
    size_t count;
    size_t capacity;

    bool heap_alloc;

    union {
        DiNodeConnection local[DI_NODE_CONNECTIONS_SMALL_SIZE];
        DiNodeConnection *heap;
    };
} DiNodeConnections;

typedef struct di_node_t {
    DiNodeConnections connections;

    DiNodeConnection *hold;
} DiNode;

void di_node_init(DiNode *node);
void di_node_destroy(DiNode *node);

void di_node_set(DiNode *node, DiTerminal *source, DiSignal move_signal);
void di_node_reset(DiNode *node, DiTerminal *source);

void di_connect(DiNode *node, DiTerminal *terminal);
void di_disconnect(DiNode *node, DiTerminal *terminal);
