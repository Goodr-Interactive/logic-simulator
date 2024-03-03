#ifndef DIGISIM_UNIT_DELAY_H
#define DIGISIM_UNIT_DELAY_H

#include <stddef.h>

#include <digisim/simulation.h>

typedef struct di_unit_simulation_t {
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

void di_unit_simulation_add(DiUnitSimulation *simulation, DiNode *node);
bool di_unit_simulation_run(DiUnitSimulation *simulation, size_t max_steps);

/**
 * Initialize a DiSimulation struct.
 *
 * @memberof DiSimulation
 * @param simulation Pointer to initialize
 */
void di_unit_simulation_init(DiUnitSimulation *simulation);

/**
 * Destroy a DiSimulation struct.
 *
 * @memberof DiSimulation
 * @param simulation Pointer to destroy
 */
void di_unit_simulation_destroy(DiUnitSimulation *simulation);

#endif // DIGISIM_UNIT_DELAY_H
