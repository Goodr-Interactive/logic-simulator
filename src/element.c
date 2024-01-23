#include <digisim/element.h>

void di_element_retain(DiElement *component) {
    component->retain(component);
}

void di_element_release(DiElement *component) {
    component->release(component);
}

void di_element_changed(DiElement *changed) {
    changed->changed(changed);
}

void di_element_init(DiElement *component) {
    di_node_list_init(&component->connections);
}

void di_element_destroy(DiElement *component) {
    di_node_list_destroy(&component->connections);
}
