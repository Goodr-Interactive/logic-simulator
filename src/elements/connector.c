#include <digisim/elements/connector.h>

void di_connector_changed(DiElement *element, DiSimulation *simulation) {
    DiConnector *connector = (DiConnector *)element;

    di_terminal_directional_read(&connector->connection_a, &connector->signal_a);
    di_terminal_directional_read(&connector->connection_b, &connector->signal_b);

    // Merge signal_a + signal_b into signal_a
    di_signal_merge(&connector->connection_a.signal, &connector->signal_a, &connector->signal_b);

    // Then move it back to signal_b
    di_signal_copy(&connector->connection_b.signal, &connector->signal_a);

    di_terminal_send(&connector->connection_a, simulation);
    di_terminal_send(&connector->connection_b, simulation);
}

void di_connector_init(DiConnector *connector, size_t bits) {
    di_element_init(&connector->element);

    connector->element.changed = di_connector_changed;

    connector->bits = bits;

    di_terminal_init(&connector->connection_a, &connector->element, bits);
    di_terminal_init(&connector->connection_b, &connector->element, bits);

    di_signal_init(&connector->signal_a, bits);
    di_signal_init(&connector->signal_b, bits);
}

void di_connector_destroy(DiConnector *connector) {
    di_terminal_destroy(&connector->connection_a);
    di_terminal_destroy(&connector->connection_b);

    di_signal_destroy(&connector->signal_a);
    di_signal_destroy(&connector->signal_b);

    di_element_destroy(&connector->element);
}
