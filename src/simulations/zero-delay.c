#include <digisim/simulations/zero-delay.h>

#include <stdlib.h>

#include <digisim/node.h>
#include <digisim/utility/node-list.h>

#define DI_SIMULATION_DEFAULT_CAPACITY 2

DiZeroSimulationEntry *di_zero_simulation_pop(DiZeroSimulation *simulation) {
    if (simulation->count <= 0) {
        return NULL;
    }

    // Top of stack
    DiZeroSimulationEntry *entry = &simulation->buffer[simulation->count - 1];

    simulation->count--;

    return entry;
}

bool di_zero_simulation_run(DiZeroSimulation *simulation, size_t max_step) {
    DiNodeList terminated;

    di_node_list_init(&terminated);

    size_t step = 0;

    while (simulation->count > 0 && step < max_step) {
        DiZeroSimulationEntry entry = *di_zero_simulation_pop(simulation);

        if (entry.depth > max_step) {
            di_node_list_add(&terminated, entry.node);
        } else {
            simulation->current_depth = entry.depth + 1;

            di_node_propagate(entry.node, &simulation->simulation);
        }
    }

    // Reset depth after simulation concludes.
    simulation->current_depth = 0;

    // We terminated early, but if we simulate again we should simulate these parts of the circuit.
    DiNode **terminated_nodes = di_node_list_values(&terminated);

    for (size_t a = 0; a < terminated.count; a++) {
        DiNode *node = terminated_nodes[a];

        di_zero_simulation_add(simulation, node);
    }

    di_node_list_destroy(&terminated);

    return step == max_step && simulation->count > 0;
}

void di_zero_simulation_clear(DiZeroSimulation *simulation) {
    simulation->count = 0;
}

void di_zero_simulation_alloc(DiZeroSimulation *simulation, size_t capacity) {
    if (capacity < simulation->capacity) {
        return;
    }

    DiZeroSimulationEntry *new_buffer = realloc(simulation->buffer, capacity * sizeof(DiZeroSimulationEntry));

    simulation->capacity = capacity;
    simulation->buffer = new_buffer;
}

void di_zero_simulation_add(DiZeroSimulation *simulation, DiNode *node) {
    if (simulation->count >= simulation->capacity) {
        size_t new_capacity = simulation->capacity * 2;

        if (new_capacity < simulation->count + 1) {
            new_capacity = simulation->count + 1;
        }

        di_zero_simulation_alloc(simulation, new_capacity);
    }

    DiZeroSimulationEntry entry = {node, simulation->current_depth};

    simulation->buffer[simulation->count] = entry;
    simulation->count++;
}

void di_zero_simulation_add_adapter(DiSimulation *simulation, DiNode *node) {
    DiZeroSimulation *zero = (DiZeroSimulation *)simulation;

    di_zero_simulation_add(zero, node);
}

void di_zero_simulation_clear_adapter(DiSimulation *simulation) {
    DiZeroSimulation *zero = (DiZeroSimulation *)simulation;

    di_zero_simulation_clear(zero);
}

bool di_zero_simulation_run_adapter(DiSimulation *simulation, size_t max_steps) {
    DiZeroSimulation *zero = (DiZeroSimulation *)simulation;

    return di_zero_simulation_run(zero, max_steps);
}

void di_zero_simulation_init(DiZeroSimulation *simulation) {
    simulation->simulation.add = di_zero_simulation_add_adapter;
    simulation->simulation.clear = di_zero_simulation_clear_adapter;
    simulation->simulation.run = di_zero_simulation_run_adapter;

    simulation->count = 0;
    simulation->capacity = DI_SIMULATION_DEFAULT_CAPACITY;
    simulation->current_depth = 0;

    simulation->buffer = malloc(simulation->capacity * sizeof(DiZeroSimulationEntry));
}

void di_zero_simulation_destroy(DiZeroSimulation *simulation) {
    free(simulation->buffer);
}
