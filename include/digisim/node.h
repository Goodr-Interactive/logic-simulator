/**
 * @file
 * @brief Interface for Circuit Wires and Connections
 */

#ifndef DIGISIM_NODE_H
#define DIGISIM_NODE_H

#include <digisim/signal.h>

#include <stdbool.h>
#include <stddef.h>

#define DI_NODE_CONNECTIONS_SMALL_SIZE 4

typedef struct di_terminal_t DiTerminal;
typedef struct di_simulation_t DiSimulation;

typedef struct di_node_connections_t {
    size_t count;
    size_t capacity;

    bool heap_alloc;

    union {
        DiTerminal *local[DI_NODE_CONNECTIONS_SMALL_SIZE];
        DiTerminal **heap;
    };
} DiNodeConnections;

typedef struct di_node_t {
    DiNodeConnections connections;

    bool error;
    DiTerminal *hold;

    bool has_signal; // available if !error && hold
    DiSignal signal;
} DiNode;

void di_node_init(DiNode *node);
void di_node_destroy(DiNode *node);

// Reads terminal values and sets node->hold
void di_node_changed(DiNode *node, DiSimulation *simulation);
// Sends node->hold to all other connections
void di_node_propagate(DiNode *node, DiSimulation *simulation);

void di_connect(DiNode *node, DiTerminal *terminal);
void di_disconnect(DiNode *node, DiTerminal *terminal);

void di_connect_simulate(DiNode *node, DiTerminal *terminal, DiSimulation *simulation);
void di_disconnect_simulate(DiNode *node, DiTerminal *terminal, DiSimulation *simulation);

#endif // DIGISIM_NODE_H
