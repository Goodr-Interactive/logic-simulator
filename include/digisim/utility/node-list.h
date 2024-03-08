/**
 * @file
 * @brief Node List Utility
 */

#ifndef DIGISIM_NODE_LIST_H
#define DIGISIM_NODE_LIST_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Locally kept capacity for DiTerminalList.
 */
#define DI_CONNECTION_LIST_SMALL_SIZE 4

typedef struct di_node_t DiNode;

/**
 * Keeps track of a list DiNode objects (wires).
 * Used to keep a graph of connections for each element.
 *
 * There are two reasons each element keeps references to their connected nodes:
 *  - Elements can be safely disconnected from their dependents when destroyed.
 *  - A entire netgraph can be discovered by jumping through connected nodes.
 */
typedef struct di_node_list_t {
    /**
     * The number of nodes that are currently in the list.
     */
    size_t count;

    /**
     * The number of nodes that can be stored in this list at this moment (either `local` or `heap`).
     */
    size_t capacity;

    /**
     * If true, `heap` stores all connected nodes. Otherwise, local stores the nodes.
     */
    bool heap_alloc;

    union {
        /**
         * Initial allocation for connected nodes, should be sufficient for most DiNodeList instances.
         */
        DiNode *local[DI_CONNECTION_LIST_SMALL_SIZE];

        /**
         * Heap allocation in case more than `DI_CONNECTION_LIST_SMALL_SIZE` nodes are connected.
         */
        DiNode **heap;
    };
} DiNodeList;

/**
 * Gets a list of DiNode values that are currently in the list.
 * There are `list->count` DiNode * pointers in the returned array.
 *
 * @memberof DiNodeList
 * @param list The node list
 * @return An array of `list->count` DiNode * pointers
 */
DiNode **di_node_list_values(DiNodeList *list);

/**
 * Adds a node to the node list.
 *
 * @memberof DiNodeList
 * @param list The list to modify
 * @param value The value to add to the list
 * @return False if the node is already in the list (and thus no modification was done)
 */
bool di_node_list_add(DiNodeList *list, DiNode *value);

/**
 * Removes a node from the node list.
 *
 * @memberof DiNodeList
 * @param list The list to modify
 * @param value The value to add to the list
 * @return False if the node was not already in the list
 */
bool di_node_list_remove(DiNodeList *list, DiNode *value);

/**
 * Remove all items in the node list.
 *
 * @memberof DiNodeList
 * @param list The list to clear
 */
void di_node_list_clear(DiNodeList *list);

/**
 * Initialize a DiNodeList struct.
 *
 * @memberof DiNodeList
 * @param list Pointer to initialize
 */
void di_node_list_init(DiNodeList *list);

/**
 * Destroy a DiNodeList struct.
 *
 * @memberof DiNodeList
 * @param list Pointer to destroy
 */
void di_node_list_destroy(DiNodeList *list);

#endif // DIGISIM_NODE_LIST_H
