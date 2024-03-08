#include <digisim/simulations/unit-delay.h>

#include <digisim/node.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DI_SIMULATION_DEFAULT_CAPACITY 2

DiNode *di_unit_simulation_pop(DiUnitSimulation *simulation) {
    if (simulation->count <= 0) {
        return NULL;
    }

    DiNode *node = simulation->buffer[simulation->start];

    simulation->start = (simulation->start + 1) % simulation->capacity;
    simulation->count--;

    return node;
}

void di_unit_simulation_step(DiUnitSimulation *simulation) {
    size_t step_size = simulation->count;

    for (size_t a = 0; a < step_size; a++) {
        assert(simulation->count > 0);

        DiNode *node = di_unit_simulation_pop(simulation);

        di_node_propagate(node, &simulation->simulation);
    }
}

bool di_unit_simulation_run(DiUnitSimulation *simulation, size_t max_step) {
    size_t step = 0;

    while (simulation->count > 0 && step < max_step) {
        di_unit_simulation_step(simulation);

        step++;
    }

    return step == max_step && simulation->count > 0;
}

void di_unit_simulation_clear(DiUnitSimulation *simulation) {
    simulation->count = 0;
}

void di_unit_simulation_alloc(DiUnitSimulation *simulation, size_t capacity) {
    // Not using realloc here as I feel that I would have to use memmove (which
    // could potentially alloc anyway).

    if (capacity < simulation->capacity) {
        return;
    }

    DiNode **new_buffer = malloc(capacity * sizeof(DiNode **));

    size_t start_count = simulation->capacity - simulation->start;

    if (start_count > simulation->count) {
        start_count = simulation->count;
    }

    memcpy(new_buffer, simulation->buffer + simulation->start, start_count * sizeof(DiNode *));

    if (start_count < simulation->count) {
        // copy second half

        size_t remaining_count = simulation->count - start_count;

        memcpy(new_buffer + start_count, simulation->buffer, remaining_count * sizeof(DiNode *));
    }

    free(simulation->buffer);

    simulation->start = 0;
    simulation->capacity = capacity;
    simulation->buffer = new_buffer;
}

void di_unit_simulation_add(DiUnitSimulation *simulation, DiNode *node) {
    if (!simulation) {
        return;
    }

    if (simulation->count >= simulation->capacity) {
        size_t new_capacity = simulation->capacity * 2;

        if (new_capacity < simulation->count + 1) {
            new_capacity = simulation->count + 1;
        }

        di_unit_simulation_alloc(simulation, new_capacity);
    }

    size_t end = (simulation->start + simulation->count) % simulation->capacity;

    simulation->buffer[end] = node;
    simulation->count++;
}

void di_unit_simulation_add_adapter(DiSimulation *simulation, DiNode *node) {
    DiUnitSimulation *unit = (DiUnitSimulation *)simulation;

    di_unit_simulation_add(unit, node);
}

void di_unit_simulation_clear_adapter(DiSimulation *simulation) {
    DiUnitSimulation *unit = (DiUnitSimulation *)simulation;

    di_unit_simulation_clear(unit);
}

bool di_unit_simulation_run_adapter(DiSimulation *simulation, size_t max_steps) {
    DiUnitSimulation *unit = (DiUnitSimulation *)simulation;

    return di_unit_simulation_run(unit, max_steps);
}

void di_unit_simulation_init(DiUnitSimulation *simulation) {
    simulation->simulation.add = di_unit_simulation_add_adapter;
    simulation->simulation.clear = di_unit_simulation_clear_adapter;
    simulation->simulation.run = di_unit_simulation_run_adapter;

    simulation->count = 0;
    simulation->capacity = DI_SIMULATION_DEFAULT_CAPACITY;
    simulation->start = 0;

    simulation->buffer = malloc(simulation->capacity * sizeof(DiNode **));
}

void di_unit_simulation_destroy(DiUnitSimulation *simulation) { free(simulation->buffer); }
