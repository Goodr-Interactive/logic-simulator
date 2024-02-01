#pragma once

#include <digisim/node-list.h>

typedef struct di_element_t DiElement;

typedef void (*DiChangedCallback)(DiElement *element);

struct di_element_t {
    DiChangedCallback changed;

    DiNodeList connections;
};

void di_element_changed(DiElement *element);

void di_element_init(DiElement *element);
void di_element_destroy(DiElement *element);
