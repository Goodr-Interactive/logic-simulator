/**
 * @file
 * @brief Interface for Circuit Elements
 */

#ifndef DIGISIM_ELEMENT_H
#define DIGISIM_ELEMENT_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Locally kept capacity for DiNodeList.
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
 * @param list The list to modify
 * @param value The value to add to the list
 * @return False if the node is already in the list (and thus no modification was done)
 */
bool di_node_list_add(DiNodeList *list, DiNode *value);

/**
 * Removes a node from the node list.
 *
 * @param list The list to modify
 * @param value The value to add to the list
 * @return False if the node was not already in the list
 */
bool di_node_list_remove(DiNodeList *list, DiNode *value);

typedef struct di_element_t DiElement;
typedef struct di_simulation_t DiSimulation;

/**
 * Callback function for when the inputs of an element has changed.
 */
typedef void (*DiChangedCallback)(DiElement *element, DiSimulation *simulation);

/**
 * Element descriptor type that all elements/components/gates should inherit.
 *
 * Find implementations in the elements/all.h header (AND/NOT/OR gates, etc.)
 * To initialize/destroy, use di_element_init/di_element_destroy.
 */
typedef struct di_element_t {
    /**
     * Called when the inputs of this elements have changed.
     * When called, use this opportunity to update outputs.
     *
     * Nullable. Invoked automatically when using a DiSimulation.
     * To invoke manually use di_element_changed.
     */
    DiChangedCallback changed;

    /**
     * List of all connections currently connected to this element.
     */
    DiNodeList connections;
} DiElement;

/**
 * Notifies the element that one of its inputs have changed and it should update its state.
 *
 * @memberof DiElement
 * @param element Element instance
 * @param simulation Simulation to propagate any output changes
 */
void di_element_changed(DiElement *element, DiSimulation *simulation);

/**
 * Initialize a DiElement struct.
 *
 * @memberof DiElement
 * @param element Pointer to initialize
 */
void di_element_init(DiElement *element);

/**
 * Destroy a DiElement struct.
 *
 * @memberof DiElement
 * @param self Pointer to destroy
 */
void di_element_destroy(DiElement *element);

#endif // DIGISIM_ELEMENT_H
