#ifndef DIGISIM_ELEMENTS_CONNECTOR_H
#define DIGISIM_ELEMENTS_CONNECTOR_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_connector_t {
    DiElement element;

    size_t bits;

    DiTerminal connection_a;
    DiTerminal connection_b;
} DiConnector;

void di_connector_init(DiConnector *connector, size_t bits);
void di_connector_destroy(DiConnector *connector);

#endif // DIGISIM_ELEMENTS_CONNECTOR_H
