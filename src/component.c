#include <digisim/component.h>

void di_component_retain(DiComponent *component) {
    component->retain(component);
}

void di_component_release(DiComponent *component) {
    component->release(component);
}

void di_component_changed(DiComponent *component) {
    component->changed(component);
}

void di_component_init(DiComponent *component) {
    di_connection_list_init(&component->connections);
}

void di_component_destroy(DiComponent *component) {
    di_connection_list_destroy(&component->connections);
}
