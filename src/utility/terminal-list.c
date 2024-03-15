#include <digisim/utility/terminal-list.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

DiTerminal **di_terminal_list_values(DiTerminalList *list) {
    if (list->heap_alloc) {
        return list->heap;
    } else {
        return list->local;
    }
}

void di_terminal_list_alloc(DiTerminalList *list, size_t new_capacity) {
    list->capacity = new_capacity;

    if (list->heap_alloc) {
        list->heap = realloc(list->heap, list->capacity * sizeof(DiTerminal *));
    } else {
        list->heap_alloc = true;

        DiTerminal **connections = malloc(list->capacity * sizeof(DiTerminal *));
        memcpy(connections, list->local, list->count * sizeof(DiTerminal *));

        list->heap = connections;
    }
}


bool di_terminal_list_add(DiTerminalList *list, DiTerminal *value) {
    if (list->count >= list->capacity) {
        size_t new_capacity = 2 * list->capacity;

        if (new_capacity < list->count + 1) {
            new_capacity = list->count + 1;
        }

        di_terminal_list_alloc(list, new_capacity);
    }

    DiTerminal **values = di_terminal_list_values(list);

    for (size_t a = 0; a < list->count; a++) {
        if (values[a] == value) {
            return false;
        }
    }

    assert(list->count < list->capacity);

    values[list->count] = value;
    list->count++;

    return true;
}

bool di_terminal_list_remove(DiTerminalList *list, DiTerminal *value) {
    size_t index = 0;

    DiTerminal **values = di_terminal_list_values(list);

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

void di_terminal_list_init(DiTerminalList *list) {
    list->count = 0;
    list->capacity = DI_NODE_CONNECTIONS_SMALL_SIZE;

    list->heap_alloc = false;
}

void di_terminal_list_destroy(DiTerminalList *list) {
    if (list->heap_alloc) {
        free(list->heap);
    }
}
