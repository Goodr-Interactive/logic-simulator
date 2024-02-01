/**
 * @file
 * @brief Interface for Circuit Elements
 */

#ifndef DIGISIM_ELEMENT_H
#define DIGISIM_ELEMENT_H

#include <stddef.h>
#include <stdbool.h>

#define DI_CONNECTION_LIST_SMALL_SIZE 4

typedef struct di_node_t DiNode;

typedef struct di_node_list_t {
    size_t count;
    size_t capacity;

    bool heap_alloc;

    union {
        DiNode *local[DI_CONNECTION_LIST_SMALL_SIZE];
        DiNode **heap;
    };
} DiNodeList;

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

#endif // DIGISIM_ELEMENT_H
