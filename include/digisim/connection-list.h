#pragma once

#include <stddef.h>
#include <stdbool.h>

#define DI_CONNECTION_LIST_SMALL_SIZE 4

typedef struct di_wire_t DiWire;

typedef struct di_connection_list_t {
    size_t count;
    size_t capacity;

    bool heap_alloc;

    union {
        DiWire *local[DI_CONNECTION_LIST_SMALL_SIZE];
        DiWire **heap;
    };
} DiConnectionList;

void di_connection_list_init(DiConnectionList *list);
void di_connection_list_destroy(DiConnectionList *list);

bool di_connection_list_add(DiConnectionList *list, DiWire *value);
bool di_connection_list_remove(DiConnectionList *list, DiWire *value);

DiWire **di_connection_list_values(DiConnectionList *list);
