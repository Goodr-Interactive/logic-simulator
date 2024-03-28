/**
* @file
* @brief Connector (Node Glue) Element
*/

#ifndef DIGISIM_ELEMENTS_CONNECTOR_H
#define DIGISIM_ELEMENTS_CONNECTOR_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * The connector element is designed to connect two DiNodes together.
 * It merges it's two terminals (connection_a, connection_b) together just like a DiNode would,
 * and then pushes the value of the merged signal back onto both terminals.
 *
 * To initialize/destroy, use di_connector_init/di_connector_destroy.
 */
typedef struct di_connector_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Bit width of the connector (DiNode size)
     */
    size_t bits;

    /**
     * First merge input for the connector
     */
    DiTerminal connection_a;

    /**
     * Second merge input for the connector
     */
    DiTerminal connection_b;

    /**
     * Temporary holding value for connection_a.
     *
     * For internal use.
     */
    DiSignal signal_a;

    /**
     * Temporary holding value for connection_b.
     *
     * For internal use.
     */
    DiSignal signal_b;
} DiConnector;

/**
 * Initialize a DiConnector struct.
 *
 * @memberof DiConnector
 * @param connector Pointer to initialize
 * @param bits Number of bits
 */
void di_connector_init(DiConnector *connector, size_t bits);

/**
 * Destroy a DiConnector struct.
 *
 * @memberof DiConnector
 * @param connector Pointer to destroy
 */
void di_connector_destroy(DiConnector *connector);

#endif // DIGISIM_ELEMENTS_CONNECTOR_H
