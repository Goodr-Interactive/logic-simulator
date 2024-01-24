#include <digisim/element.h>

#include <string.h>

void di_element_changed(DiElement *element) {
    if (element->changed) {
        element->changed(element);
    }
}

void di_element_init(DiElement *component) {
    memset(component, 0, sizeof(DiElement));

    di_node_list_init(&component->connections);
}

void di_element_destroy(DiElement *component) {
    di_node_list_destroy(&component->connections);
}
