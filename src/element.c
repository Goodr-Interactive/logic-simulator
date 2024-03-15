#include <digisim/element.h>

#include <digisim/node.h>
#include <digisim/terminal.h>

#include <string.h>

void di_element_changed(DiElement *element, DiSimulation *simulation) {
    if (element->changed) {
        element->changed(element, simulation);
    }
}

void di_element_reset(DiElement *element) {
    if (element->reset) {
        element->reset(element);
    }
}

void di_element_disconnect_all(DiElement *element, DiSimulation *simulation) {
    DiTerminal **terminals = di_terminal_list_values(&element->connections);

    for (size_t a = 0; a < element->connections.count; a++) {
        DiTerminal *terminal = terminals[a];

        di_disconnect_simulate(terminal->node, terminal, simulation);
    }
}

void di_element_init(DiElement *component) {
    memset(component, 0, sizeof(DiElement));

    di_terminal_list_init(&component->connections);
}

void di_element_destroy(DiElement *component) { di_terminal_list_destroy(&component->connections); }
