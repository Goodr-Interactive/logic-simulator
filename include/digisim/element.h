#pragma once

#include <digisim/node-list.h>

typedef struct di_element_t DiElement;
typedef struct di_simulation_t DiSimulation;

typedef void (*DiChangedCallback)(DiElement *element, DiSimulation *simulation);

struct di_element_t {
    DiChangedCallback changed;

    DiNodeList connections;
};

void di_element_changed(DiElement *element, DiSimulation *simulation);

void di_element_init(DiElement *element);
void di_element_destroy(DiElement *element);
