#ifndef DIGISIM_ZERO_DELAY_H
#define DIGISIM_ZERO_DELAY_H

#include <stddef.h>

#include <digisim/element.h>
#include <digisim/simulation.h>

typedef struct di_zero_simulation_entry_t {
    DiNode *node;
    size_t depth;
} DiZeroSimulationEntry;

typedef struct di_zero_simulation_t {
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
     * Current simulation depth.
     *
     * Set to zero on di_zero_simulation_run.
     * Before calling propagate, this simulation_depth is set to the depth of the current node.
     * For every call to di_zero_simulation_add, the entry is added with a depth of current_depth.
     */
    size_t current_depth;

    /**
     * A heap allocated buffer containing all nodes in queue.
     *
     * Not all DiNodes are initialized, as the buffer is circular.
     */
    DiZeroSimulationEntry *buffer;

    DiNodeList terminated;
} DiZeroSimulation;

void di_zero_simulation_add(DiZeroSimulation *simulation, DiNode *node);
bool di_zero_simulation_run(DiZeroSimulation *simulation, size_t max_steps);

/**
 * Initialize a DiSimulation struct.
 *
 * @memberof DiSimulation
 * @param simulation Pointer to initialize
 */
void di_zero_simulation_init(DiZeroSimulation *simulation);

/**
 * Destroy a DiSimulation struct.
 *
 * @memberof DiSimulation
 * @param simulation Pointer to destroy
 */
void di_zero_simulation_destroy(DiZeroSimulation *simulation);

#endif // DIGISIM_ZERO_DELAY_H
