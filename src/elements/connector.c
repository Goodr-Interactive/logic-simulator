#include <digisim/elements/connector.h>

void di_connector_changed(DiElement *element, DiSimulation *simulation) {
    DiConnector *connector = (DiConnector *)element;

    DiSignal *signal_a = di_terminal_read(&connector->connection_a);
    DiSignal *signal_b = di_terminal_read(&connector->connection_b);

    // Merge signal_a + signal_b into signal_a
    di_signal_merge(&connector->connection_a.signal, signal_a, signal_b);

    // Then move it back to signal_b
    di_signal_copy(&connector->connection_b.signal, signal_a);

    di_terminal_send(&connector->connection_a, simulation);
    di_terminal_send(&connector->connection_b, simulation);
}

void di_connector_init(DiConnector *connector, size_t bits) {
    di_element_init(&connector->element);

    connector->element.changed = di_connector_changed;

    connector->bits = bits;

    di_terminal_init(&connector->connection_a, &connector->element, bits);
    di_terminal_init(&connector->connection_b, &connector->element, bits);
}

void di_connector_destroy(DiConnector *connector) {
    di_terminal_destroy(&connector->connection_a);
    di_terminal_destroy(&connector->connection_b);

    di_element_destroy(&connector->element);
}
