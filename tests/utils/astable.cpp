#include <utils/astable.h>

Astable::Astable() {
    di_input_init(&input, 1);

    for (size_t a = 0; a < 3; a++) {
        di_node_init(&nodes[a], 1);
        di_not_init(&gates[a], 1);
    }

    for (size_t a = 0; a < 3; a++) {
        size_t next = (a + 1) % 3;

        di_connect(&nodes[a], &gates[a].output);
        di_connect(&nodes[a], &gates[next].input);
    }

    di_connect(&nodes[0], &input.output);
}

Astable::~Astable() {
    for (auto &node : nodes) {
        di_node_destroy(&node);
    }

    for (auto &gate : gates) {
        di_not_destroy(&gate);
    }

    di_input_destroy(&input);

}
