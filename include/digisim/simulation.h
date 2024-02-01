#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct di_node_t DiNode;

typedef struct di_simulation_t {
    size_t count;
    size_t capacity;

    size_t start;

    DiNode **buffer;
} DiSimulation;

void di_simulation_step(DiSimulation *simulation);
bool di_simulation_flush(DiSimulation *simulation, size_t max_step);

// Accepts simulation == NULL
void di_simulation_add(DiSimulation *simulation, DiNode *node);

void di_simulation_init(DiSimulation *simulation);
void di_simulation_destroy(DiSimulation *simulation);
