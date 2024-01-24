#include <digisim/simulation.h>

#include <digisim/node.h>

#include <stdlib.h>
#include <string.h>

#define DI_SIMULATION_DEFAULT_CAPACITY 32

DiNode *di_simulation_pop(DiSimulation *simulation) {
    if (simulation->count <= 0) {
        return NULL;
    }

    DiNode *node = simulation->buffer[simulation->start];

    simulation->start++;
    simulation->count--;

    return node;
}

void di_simulation_step(DiSimulation *simulation) {
    size_t step_size = simulation->count;

    for (size_t a = 0; a < step_size; a++) {
        DiNode *item = di_simulation_pop(simulation);

        // DiNode propagate
    }
}

bool di_simulation_flush(DiSimulation *simulation, size_t max_step) {
    size_t step = 0;

    while (simulation->count > 0 && step < max_step) {
        di_simulation_step(simulation);

        step++;
    }

    return step == max_step && simulation->count > 0;
}

void di_simulation_alloc(DiSimulation *simulation, size_t capacity) {
    // Not using realloc here as I feel that I would have to use memmove (which could potentially alloc anyway).

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

void di_simulation_add(DiSimulation *simulation, DiNode *node) {
    if (simulation->count >= simulation->capacity) {
        size_t new_capacity = simulation->capacity;

        if (new_capacity < simulation->count + 1) {
            new_capacity = simulation->count + 1;
        }

        di_simulation_alloc(simulation, new_capacity);
    }

    size_t end = (simulation->start + simulation->count) % simulation->capacity;

    simulation->buffer[end] = node;
    simulation->count++;
}

void di_simulation_init(DiSimulation *simulation) {
    simulation->count = 0;
    simulation->capacity = DI_SIMULATION_DEFAULT_CAPACITY;
    simulation->start = 0;

    simulation->buffer = malloc(simulation->capacity * sizeof(DiNode **));
}

void di_simulation_destroy(DiSimulation *simulation) {
    free(simulation->buffer);
}
