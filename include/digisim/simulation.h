/**
 * @file
 * @brief Interface for Simulation and Propagation
 */

#ifndef DIGISIM_SIMULATION_H
#define DIGISIM_SIMULATION_H

#include <stdbool.h>
#include <stddef.h>

typedef struct di_node_t DiNode;
typedef struct di_simulation_t DiSimulation;

typedef void (*DiSimulationAddCallback)(DiSimulation *simulation, DiNode *node);
typedef bool (*DiSimulationRunCallback)(DiSimulation *simulation, size_t max_depth);

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
    DiSimulationAddCallback add;
    DiSimulationRunCallback run;
} DiSimulation;

/**
 * Propagates everything until the buffer is empty, or until `max_step` steps have been taken.
 * Astable circuits may never finish simulating, so a max_step is required.
 *
 * @memberof DiSimulation
 * @param simulation The simulation queue
 * @param max_depth The maximum number of wires changes to pursue before quiting
 * @return True if `max_step`s have passed and the simulation has not settled
 */
bool di_simulation_run(DiSimulation *simulation, size_t max_depth);

/**
 * Adds a node to the propagation queue (to be simulated later).
 *
 * @memberof DiSimulation
 * @param simulation The simulation queue (nullable)
 * @param node The node to propagate
 */
void di_simulation_add(DiSimulation *simulation, DiNode *node);

DiSimulation *di_simulation_create();
void di_simulation_free(DiSimulation *simulation);

#endif // DIGISIM_SIMULATION_H
