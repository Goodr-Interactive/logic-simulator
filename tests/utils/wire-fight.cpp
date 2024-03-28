#include <utils/wire-fight.h>

WireFight::WireFight(size_t bits) {
    di_input_init(&fighter_a, bits);
    di_input_init(&fighter_b, bits);

    di_output_init(&value, bits);

    di_node_init(&connector, bits);

    di_connect(&connector, &fighter_a.output);
    di_connect(&connector, &fighter_b.output);
    di_connect(&connector, &value.input);
}

WireFight::~WireFight() {
    di_node_destroy(&connector);

    di_input_destroy(&fighter_a);
    di_input_destroy(&fighter_b);

    di_output_destroy(&value);
}