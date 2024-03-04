/**
 * @file
 * @brief Zero Delay Simulation Implementation
 */

#ifndef DIGISIM_ZERO_DELAY_H
#define DIGISIM_ZERO_DELAY_H

#include <stddef.h>

#include <digisim/element.h>
#include <digisim/simulation.h>

/**
 * Entry for a zero simulation stack.
 */
typedef struct di_zero_simulation_entry_t {
    /**
     * The node to simulate.
     */
    DiNode *node;

    /**
     * The depth that this simulation node was discovered.
     */
    size_t depth;
} DiZeroSimulationEntry;

/**
 * A simulation implementation that does not associate any delay with element processing.
 *
 * There's no specified order that wires will be processed.
 * Use a unit delay simulation if you need some structure to simulation events.
 *
 * Similar to DFS in exploration.
 */
typedef struct di_zero_simulation_t {
    /**
     * Simulation descriptor
     */
    DiSimulation simulation;

    /**
     * The number of nodes in stack to simulate.
     */
    size_t count;

    /**
     * The capacity of the stack.
     */
    size_t capacity;

    /**
     * Current simulation depth.
     *
     * Set to zero on di_zero_simulation_run.
     * Before calling propagate, this simulation_depth is set to the depth of the current node.
     * For every call to di_zero_simulation_add, the entry is added with a depth of current_depth.
     */
    size_t current_depth;

    /**
     * A heap allocated buffer containing all nodes in stack.
     *
     * Not all DiNodes are initialized, as the buffer is circular.
     */
    DiZeroSimulationEntry *buffer;
} DiZeroSimulation;

/**
 * Add a node to a simulation. Counterpart to `di_simulation_add`.
 *
 * @memberof DiZeroSimulation
 * @param simulation The zero simulation
 * @param node The node to add to the unit simulation
 */
void di_zero_simulation_add(DiZeroSimulation *simulation, DiNode *node);

/**
 * Run a zero simulation. This method is automatically called by `di_simulation_run`.
 *
 * @memberof DiZeroSimulation
 * @param simulation The zero simulation to run
 * @param max_steps The maximum exploration depth
 * @return True if the simulation ended early since it ran out of steps.
 */
bool di_zero_simulation_run(DiZeroSimulation *simulation, size_t max_steps);

/**
 * Initialize a DiZeroSimulation struct.
 *
 * @memberof DiZeroSimulation
 * @param simulation Pointer to initialize
 */
void di_zero_simulation_init(DiZeroSimulation *simulation);

/**
 * Destroy a DiZeroSimulation struct.
 *
 * @memberof DiZeroSimulation
 * @param simulation Pointer to destroy
 */
void di_zero_simulation_destroy(DiZeroSimulation *simulation);

#endif // DIGISIM_ZERO_DELAY_H
