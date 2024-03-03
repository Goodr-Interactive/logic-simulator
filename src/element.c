#include <digisim/element.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void di_node_list_init(DiNodeList *list) {
    list->count = 0;
    list->capacity = DI_CONNECTION_LIST_SMALL_SIZE;
    list->heap_alloc = false;
}

void di_node_list_destroy(DiNodeList *list) {
    assert(list->count <= 0);

    if (list->heap_alloc) {
        free(list->heap);
    }
}

DiNode **di_node_list_values(DiNodeList *list) {
    if (list->heap_alloc) {
        return list->heap;
    } else {
        return list->local;
    }
}

void di_connection_list_alloc(DiNodeList *list, size_t new_capacity) {
    list->capacity = new_capacity;

    if (list->heap_alloc) {
        list->heap = realloc(list->heap, list->capacity * sizeof(DiNode *));
    } else {
        list->heap_alloc = true;
        DiNode **wires = malloc(list->capacity * sizeof(DiNode *));
        memcpy(wires, list->local, list->count * sizeof(DiNode *));

        list->heap = wires;
    }
}

bool di_node_list_add(DiNodeList *list, DiNode *value) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;

        if (new_capacity < list->count + 1) {
            new_capacity = list->count + 1;
        }

        di_connection_list_alloc(list, new_capacity);
    }

    DiNode **values = di_node_list_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a] == value) {
            return false;
        }
    }

    values[list->count] = value;
    list->count++;

    assert(list->count <= list->capacity);

    return true;
}

void di_node_list_clear(DiNodeList *list) {
    list->count = 0;
}

bool di_node_list_remove(DiNodeList *list, DiNode *value) {
    DiNode **values = di_node_list_values(list);

    size_t index = 0;

    for (; index < list->count; index++) {
        if (values[index] == value) {
            break;
        }
    }

    if (index >= list->count) {
        return false;
    }

    for (; index < list->count; index++) {
        values[index - 1] = values[index];
    }

    list->count--;

    return true;
}

void di_element_changed(DiElement *element, DiSimulation *simulation) {
    if (element->changed) {
        element->changed(element, simulation);
    }
}

void di_element_init(DiElement *component) {
    memset(component, 0, sizeof(DiElement));

    di_node_list_init(&component->connections);
}

void di_element_destroy(DiElement *component) { di_node_list_destroy(&component->connections); }
