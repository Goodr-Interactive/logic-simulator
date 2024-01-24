#pragma once

#include <stdbool.h>
#include <stddef.h>

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

void di_node_list_init(DiNodeList *list);
void di_node_list_destroy(DiNodeList *list);

bool di_node_list_add(DiNodeList *list, DiNode *value);
bool di_node_list_remove(DiNodeList *list, DiNode *value);

DiNode **di_node_list_values(DiNodeList *list);
