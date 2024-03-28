/**
 * @file
 * @brief Terminal List Utility
 */

#ifndef DIGISIM_TERMINAL_LIST_H
#define DIGISIM_TERMINAL_LIST_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Locally kept capacity for DiNodeConnections.
 */
#define DI_NODE_CONNECTIONS_SMALL_SIZE 4

typedef struct di_terminal_t DiTerminal;

/**
 * Keeps track of all connections to this node.
 *
 * DiTerminals on this list are propagated to when a node changes value.
 */
typedef struct di_terminal_list_t {
    /**
     * The number of terminals currently connected to the node.
     */
    size_t count;

    /**
     * The number of terminals that can be stored in this list at this moment (either `local` or `heap`).
     */
    size_t capacity;

    /**
     * If true, `heap` stores all connected terminals. Otherwise, local stores the terminals.
     */
    bool heap_alloc;

    union {
        /**
         * Initial allocation for connected nodes, should be sufficient for most DiNodeConnections instances.
         */
        DiTerminal *local[DI_NODE_CONNECTIONS_SMALL_SIZE];

        /**
         * Heap allocation in case more than `DI_NODE_CONNECTIONS_SMALL_SIZE` connections are made.
         */
        DiTerminal **heap;
    };
} DiTerminalList;

/**
 * Gets a list of DiTerminal values that are currently in the list.
 * There are `list->count` DiTerminal * pointers in the returned array.
 *
 * @memberof DiTerminalList
 * @param list The terminal list
 * @return An array of `list->count` DiTerminal * pointers
 */
DiTerminal **di_terminal_list_values(DiTerminalList *list);

/**
 * Adds a terminal to the terminal list.
 *
 * @memberof DiTerminalList
 * @param list The list to modify
 * @param value The value to add to the list
 * @return False if the terminal is already in the list (and thus no modification was done)
 */
bool di_terminal_list_add(DiTerminalList *list, DiTerminal *value);

/**
 * Removes a terminal from the terminal list.
 *
 * @memberof DiTerminalList
 * @param list The list to modify
 * @param value The value to add to the list
 * @return False if the terminal was not already in the list
 */
bool di_terminal_list_remove(DiTerminalList *list, DiTerminal *value);

/**
 * Initialize a DiTerminalList struct.
 *
 * @memberof DiTerminalList
 * @param list Pointer to initialize
 */
void di_terminal_list_init(DiTerminalList *list);

/**
 * Destroy a DiTerminalList struct.
 *
 * @memberof DiTerminalList
 * @param list Pointer to destroy
 */
void di_terminal_list_destroy(DiTerminalList *list);

#endif // DIGISIM_TERMINAL_LIST_H
