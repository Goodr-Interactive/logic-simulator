/**
 * @file
 * @brief Interface for Circuit Elements
 */

#ifndef DIGISIM_ELEMENT_H
#define DIGISIM_ELEMENT_H

#include <digisim/utility/terminal-list.h>

#include <stdbool.h>
#include <stddef.h>

typedef struct di_node_t DiNode;
typedef struct di_element_t DiElement;
typedef struct di_simulation_t DiSimulation;

/**
 * Callback function for when the inputs of an element has changed.
 */
typedef void (*DiChangedCallback)(DiElement *element, DiSimulation *simulation);

/**
 * Callback function for resetting an elements internal state (ex. D-Latch latched values).
 */
typedef void (*DiResetCallback)(DiElement *element, DiSimulation *simulation);

/**
 * Element descriptor type that all elements/components/gates should inherit.
 * Elements must be pinned to a memory location, as they contain references to itself.
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
     * Called by the user when the internal state (sequential) should be reset.
     */
    DiResetCallback reset;

    /**
     * List of all terminals that are part of this element.
     */
    DiTerminalList connections;
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
 * Resets the internal state of the element.
 * For example, the internal value of a D-Latch.
 *
 * @memberof DiElement
 * @param element Element instance
 * @param simulation Simulation to propagate any output changes from internal state
 */
void di_element_reset(DiElement *element, DiSimulation *simulation);

/**
 * Disconnects all connected terminals and nodes from this element.
 *
 * @memberof DiElement
 * @param element The element instance to disconnect
 * @param simulation The simulation to propagate any changes (nullable)
 */
void di_element_disconnect_all(DiElement *element, DiSimulation *simulation);

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
