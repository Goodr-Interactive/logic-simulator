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

/**
 * Callback to add a node to a simulation.
 */
typedef void (*DiSimulationAddCallback)(DiSimulation *simulation, DiNode *node);

/**
 * Callback to clear the simulation queue.
 */
typedef void (*DiSimulationClearCallback)(DiSimulation *simulation);

/**
 * Callback to run this simulation.
 */
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
    /**
     * Callback that's called when an element wants to propagate a wire value.
     */
    DiSimulationAddCallback add;

    /**
     * Callback that's called by the user to clear the simulation queue.
     */
    DiSimulationClearCallback clear;

    /**
     * Callback that's called by the user to start simulating.
     */
    DiSimulationRunCallback run;
} DiSimulation;

/**
 * Propagates everything until the buffer is empty, or until `max_step` steps have been taken.
 * Astable circuits may never finish simulating, so a max_step is required.
 *
 * @memberof DiSimulation
 * @param simulation The simulation
 * @param max_depth The maximum number of wires changes to pursue before quiting
 * @return True if `max_step`s have passed and the simulation has not settled
 */
bool di_simulation_run(DiSimulation *simulation, size_t max_depth);

/**
 * Clears the internal simulation queue.
 * Use to discard any potential queued simulation changes (ex. if the circuit has been reset).
 *
 * @memberof DiSimulation
 * @param simulation The simulation to clear
 */
void di_simulation_clear(DiSimulation *simulation);

/**
 * Adds a node to the propagation queue (to be simulated later).
 *
 * @memberof DiSimulation
 * @param simulation The simulation object (nullable)
 * @param node The node to propagate
 */
void di_simulation_add(DiSimulation *simulation, DiNode *node);

/**
 * Allocates and returns a pointer to a default simulation.
 *
 * By default this is the zero delay simulation (digisim/simulations/zero-delay.h).
 *
 * Free this pointer using di_simulation_free.
 *
 * @return A pointer to a DiSimulation object.
 */
DiSimulation *di_simulation_create();

/**
 * Frees a pointer created by `di_simulation_create`.
 *
 * @param simulation A pointer allocated by `di_simulation_create`.
 */
void di_simulation_free(DiSimulation *simulation);

#endif // DIGISIM_SIMULATION_H
