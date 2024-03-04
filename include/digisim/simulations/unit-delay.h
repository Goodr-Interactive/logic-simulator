/**
* @file
* @brief Unit Delay Simulation Implementation
*/

#ifndef DIGISIM_UNIT_DELAY_H
#define DIGISIM_UNIT_DELAY_H

#include <stddef.h>

#include <digisim/simulation.h>

/**
 * A simulation implementation that associates a delay of 1 unit to every element.
 *
 * This means wires will finish propagating their changes closer to their origin before moving further away.
 *
 * Similar to BFS in exploration.
 */
typedef struct di_unit_simulation_t {
    /**
     * Simulation descriptor
     */
    DiSimulation simulation;

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
} DiUnitSimulation;

/**
 * Add a node to a simulation. Counterpart to `di_simulation_add`.
 *
 * @memberof DiUnitSimulation
 * @param simulation The unit simulation
 * @param node The node to add to the unit simulation
 */
void di_unit_simulation_add(DiUnitSimulation *simulation, DiNode *node);

/**
 * Run a unit simulation. This method is automatically called by `di_simulation_run`.
 *
 * @memberof DiUnitSimulation
 * @param simulation The unit simulation to run
 * @param max_steps The maximum exploration depth
 * @return True if the simulation ended early since it ran out of steps.
 */
bool di_unit_simulation_run(DiUnitSimulation *simulation, size_t max_steps);

/**
 * Initialize a DiUnitSimulation struct.
 *
 * @memberof DiUnitSimulation
 * @param simulation Pointer to initialize
 */
void di_unit_simulation_init(DiUnitSimulation *simulation);

/**
 * Destroy a DiUnitSimulation struct.
 *
 * @memberof DiUnitSimulation
 * @param simulation Pointer to destroy
 */
void di_unit_simulation_destroy(DiUnitSimulation *simulation);

#endif // DIGISIM_UNIT_DELAY_H
