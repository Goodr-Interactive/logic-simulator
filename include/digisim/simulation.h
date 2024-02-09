/**
 * @file
 * @brief Interface for Simulation and Propagation
 */

#ifndef DIGISIM_SIMULATION_H
#define DIGISIM_SIMULATION_H

#include <stdbool.h>
#include <stddef.h>

typedef struct di_node_t DiNode;

/**
 * Keeps track of simulation parameters and holds a queue of nodes to "propagate."
 *
 * When a change to a node occurs, propagation of the value waits until the simulation reaches it.
 * So changes are put into the DiSimulation struct to be visited later.
 *
 * To start a simulation, use `di_simulation_step` or `di_simulation_run`.
 *
 * To initialize/destroy, use di_simulation_init/di_simulation_destroy.
 */
typedef struct di_simulation_t {
    /**
     * The number of nodes in queue to simulate.
     */
    size_t count;

    /**
     * The capacity of the queue.
     */
    size_t capacity;

    /**
     * Index of the first DiNode * in queue to simulate.
     *
     * This is a circular buffer, so start is moved forward when a node is popped.
     */
    size_t start;

    /**
     * A heap allocated buffer containing all nodes in queue.
     *
     * Not all DiNodes are initialized, as the buffer is circular.
     */
    DiNode **buffer;
} DiSimulation;

/**
 * Propagates everything currently in the buffer, leaving all new changes created from this propagation in the queue.
 *
 * @memberof DiSimulation
 * @param simulation The simulation queue
 */
void di_simulation_step(DiSimulation *simulation);

/**
 * Propagates everything until the buffer is empty, or until `max_step` steps have been taken.
 * Astable circuits may never finish simulating, so a max_step is required.
 *
 * @memberof DiSimulation
 * @param simulation The simulation queue
 * @param max_step The maximum number of steps to take before quiting
 * @return True if `max_step`s have passed and the simulation has not settled
 */
bool di_simulation_run(DiSimulation *simulation, size_t max_step);

/**
 * Adds a node to the propagation queue (to be simulated later).
 *
 * @memberof DiSimulation
 * @param simulation The simulation queue (nullable)
 * @param node The node to propagate
 */
void di_simulation_add(DiSimulation *simulation, DiNode *node);

/**
 * Initialize a DiSimulation struct.
 *
 * @memberof DiSimulation
 * @param simulation Pointer to initialize
 */
void di_simulation_init(DiSimulation *simulation);

/**
 * Destroy a DiSimulation struct.
 *
 * @memberof DiSimulation
 * @param simulation Pointer to destroy
 */
void di_simulation_destroy(DiSimulation *simulation);

#endif // DIGISIM_SIMULATION_H
