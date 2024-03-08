#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/simulations/unit-delay.h>
#include <digisim/node.h>
#include <digisim/elements/all.h>
}

TEST_CASE("Test Register Pass on Clock") {
    DiInput data;
    DiInput clock;
    DiOutput value;

    DiRegister reg;

    DiNode dataReg;
    DiNode clockReg;
    DiNode valueReg;

    di_input_init(&data, 8);
    di_input_init(&clock, 1);
    di_output_init(&value, 8);

    di_register_init(&reg, 8);

    di_node_init(&dataReg, 8);
    di_node_init(&clockReg, 1);
    di_node_init(&valueReg, 8);

    di_connect(&dataReg, &data.output);
    di_connect(&dataReg, &reg.data);

    di_connect(&clockReg, &clock.output);
    di_connect(&clockReg, &reg.clock);

    di_connect(&valueReg, &reg.value);
    di_connect(&valueReg, &value.input);

    DiSimulation *simulation = di_simulation_create();

    bool x[8] = {
        true, false, true, false, true, false, true, false
    };

    for (size_t a = 0; a < 8; a++) {
        di_signal_set(&data.signal, a, static_cast<DiBit>(x[a]));
    }

    di_input_emit(&data, simulation);
    di_input_set_bit(&clock, 0, DI_BIT_LOW, simulation);
    di_simulation_run(simulation, 100);

    di_input_set_bit(&clock, 0, DI_BIT_HIGH, simulation);
    di_simulation_run(simulation, 100);

    for (size_t a = 0; a < 8; a++) {
        REQUIRE(di_output_get_bit(&value, a) == static_cast<DiBit>(x[a]));
    }

    di_simulation_free(simulation);

    di_node_destroy(&dataReg);
    di_node_destroy(&clockReg);
    di_node_destroy(&valueReg);

    di_register_destroy(&reg);

    di_input_destroy(&data);
    di_input_destroy(&clock);
    di_output_destroy(&value);
}
