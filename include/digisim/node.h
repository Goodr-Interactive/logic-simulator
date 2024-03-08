/**
 * @file
 * @brief Interface for Circuit Wires and Connections
 */

#ifndef DIGISIM_NODE_H
#define DIGISIM_NODE_H

#include <digisim/signal.h>
#include <digisim/utility/terminal-list.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct di_terminal_t DiTerminal;
typedef struct di_simulation_t DiSimulation;

/**
 * Represents a wire in a circuit, connecting multiple elements together.
 *
 * A wire attaches to an element via a DiTerminal (an input/output port).
 * To connect a DiNode to a DiTerminal, use `di_connect` or `di_connect_simulate`.
 *
 * To initialize/destroy, use di_node_init/di_node_destroy.
 */
typedef struct di_node_t {
    /**
     * Holds all terminals that are currently connected to the wire.
     */
    DiTerminalList connections;

    /**
     * The number of bits carried by this wire.
     */
    size_t bits;

    /**
     * The value that this wire is held to.
     *
     * Only read this value if `has_signal` is true.
     */
    DiSignal signal;

    /**
     * The last value of the signal, before a change was propagated.
     *
     * It is recommended to avoid using this value, it is here to avoid allocations in di_node_propagate.
     */
    DiSignal last_signal;
} DiNode;

/**
 * Initialize the DiNode struct.
 *
 * @memberof DiNode
 * @param node Pointer to initialize
 * @param bits Number of bits carried by this wire
 */
void di_node_init(DiNode *node, size_t bits);

/**
 * Destroy a DiNode struct.
 *
 * @memberof DiNode
 * @param node Pointer to destroy
 */
void di_node_destroy(DiNode *node);

/**
 * Recalculates the value of `hold`, `has_signal`, `signal` based on current terminal values.
 *
 * Automatically called by DiTerminal in most cases.
 *
 * @memberof DiNode
 * @param node Node instance
 * @param simulation Simulation to queue the di_node_propagate call (nullable)
 */
void di_node_changed(DiNode *node, DiSimulation *simulation);

/**
 * Propagates a change in the wire value to all connected DiTerminals.
 *
 * Calls di_element_changed on the connected terminals.
 *
 * @memberof DiNode
 * @param node Node instance
 * @param simulation Simulation to queue all resulting propagations (nullable)
 */
void di_node_propagate(DiNode *node, DiSimulation *simulation);

/**
 * Connects a node to a terminal.
 *
 * @memberof DiNode
 * @param node The node to connect
 * @param terminal The terminal to connect
 */
void di_connect(DiNode *node, DiTerminal *terminal);

/**
 * Disconnects a node from a terminal.
 *
 * @memberof DiNode
 * @param node The node to disconnect
 * @param terminal The terminal to disconnect
 */
void di_disconnect(DiNode *node, DiTerminal *terminal);

/**
 * Connects a node to a terminal and queues up a propagation in a simulation.
 * Useful for when this new wire connection might have consequences on an active simulation.
 * Generally, use this method over `di_connect` if you already have a DiSimulation object.
 *
 * Otherwise, it is recommended to add any `DiInput`s to the simulation with `di_input_emit`.
 *
 * `di_connect(node, terminal)` is shorthand for `di_connect_simulate(node, terminal, NULL)`.
 *
 * @memberof DiNode
 * @param node The node to connect
 * @param terminal The terminal to connect
 * @param simulation The simulation to queue a propagation to node (to evaluate any consequences)
 */
void di_connect_simulate(DiNode *node, DiTerminal *terminal, DiSimulation *simulation);

/**
 * Disconnects a node from a terminal.
 * Useful for when disconnecting this wire might have consequences on an active simulation.
 * Generally, use this method over `di_disconnect` if you already have a DiSimulation object.
 *
 * Otherwise, it is recommended to add any `DiInput`s to the simulation with `di_input_emit`.
 *
 * `di_disconnect(node, terminal)` is shorthand for `di_disconnect_simulate(node, terminal, NULL)`.
 *
 * @memberof DiNode
 * @param node The node to disconnect
 * @param terminal The terminal to disconnect
 * @param simulation
 */
void di_disconnect_simulate(DiNode *node, DiTerminal *terminal, DiSimulation *simulation);

#endif // DIGISIM_NODE_H
