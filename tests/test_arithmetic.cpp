#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

TEST_CASE("Test Constant Has Value") {
    DiOutput out;
    DiNode valueToOut;
    DiConstant value;

    di_output_init(&out, 16);
    di_node_init(&valueToOut, 16);
    di_constant_init(&value, 16, 0x3FC7);

    di_connect(&valueToOut, &out.input);
    di_connect(&valueToOut, &value.output);

    DiSimulation *simulation = di_simulation_create();

    // Alternatively, shake the circuit by making every element fire a "changed" event.
    di_constant_emit(&value, simulation);

    di_simulation_run(simulation, 100);

    REQUIRE((di_signal_get_values(&out.signal)[0] & 0xFFFF) == 0x3FC7);

    di_simulation_free(simulation);

    di_node_destroy(&valueToOut);

    di_output_destroy(&out);
    di_constant_destroy(&value);
}
