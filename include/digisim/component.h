#pragma once

#include <digisim/connection-list.h>

typedef struct di_component_t DiComponent;

typedef void (*DiRetainCallback)(DiComponent *component);
typedef void (*DiReleaseCallback)(DiComponent *component);

typedef void (*DiChangedCallback)(DiComponent *component);

struct di_component_t {
    DiRetainCallback retain;
    DiReleaseCallback release;
    DiChangedCallback changed;

    DiConnectionList connections;
};

void di_component_retain(DiComponent *component);
void di_component_release(DiComponent *component);
void di_component_changed(DiComponent *changed);

void di_component_init(DiComponent *component);
void di_component_destroy(DiComponent *component);
