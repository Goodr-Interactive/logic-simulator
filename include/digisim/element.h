#pragma once

#include <digisim/node-list.h>

typedef struct di_element_t DiElement;

typedef void (*DiRetainCallback)(DiElement *element);
typedef void (*DiReleaseCallback)(DiElement *element);

typedef void (*DiChangedCallback)(DiElement *element);

struct di_element_t {
    DiRetainCallback retain;
    DiReleaseCallback release;
    DiChangedCallback changed;

    DiNodeList connections;
};

void di_element_retain(DiElement *element);
void di_element_release(DiElement *element);
void di_element_changed(DiElement *changed);

void di_element_init(DiElement *element);
void di_element_destroy(DiElement *element);
