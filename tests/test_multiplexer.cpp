#include <catch2/catch_all.hpp>

#include <utils/multiplexer.h>

extern "C" {
#include <digisim/simulations/unit-delay.h>
}

TEST_CASE("Test Multiplexer Creation") { Multiplexer multiplexer; }

TEST_CASE("Test Multiplexer Step Low") {
    Multiplexer multiplexer;

    DiUnitSimulation unit;
    di_unit_simulation_init(&unit);

    multiplexer.addInputs(&unit.simulation);

    REQUIRE(unit.count == 3);

    di_unit_simulation_run(&unit, 1);

    REQUIRE(multiplexer.and1.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and1.output.signal, 0) == DI_BIT_LOW);

    REQUIRE(multiplexer.and2.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and2.output.signal, 0) == DI_BIT_LOW);

    di_unit_simulation_destroy(&unit);
}

TEST_CASE("Test Multiplexer Step High") {
    Multiplexer multiplexer;

    DiUnitSimulation unit;

    di_unit_simulation_init(&unit);

    di_input_set_bit(&multiplexer.c, 0, DI_BIT_HIGH, &unit.simulation);
    di_input_set_bit(&multiplexer.b, 0, DI_BIT_LOW, &unit.simulation);
    di_input_set_bit(&multiplexer.a, 0, DI_BIT_LOW, &unit.simulation);

    REQUIRE(unit.count == 3);

    di_unit_simulation_run(&unit, 2);

    REQUIRE(multiplexer.and1.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and1.output.signal, 0) == DI_BIT_LOW);

    REQUIRE(multiplexer.and2.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and2.output.signal, 0) == DI_BIT_HIGH);

    di_unit_simulation_destroy(&unit);
}

DiBit testMultiplexer(DiBit x, DiBit y, DiBit s) {
    Multiplexer multiplexer;

    DiSimulation *simulation = di_simulation_create();

    di_input_set_bit(&multiplexer.c, 0, x, simulation);
    di_input_set_bit(&multiplexer.b, 0, s, simulation);
    di_input_set_bit(&multiplexer.a, 0, y, simulation);

    di_simulation_run(simulation, 100);

    DiBit value = di_signal_get(&multiplexer.f.signal, 0);

    di_simulation_free(simulation);

    return value;
}

TEST_CASE("Test Multiplexer Output Pick High") {
    REQUIRE(testMultiplexer(DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW) == DI_BIT_HIGH);
}

TEST_CASE("Test Multiplexer Output X Toggle") {
    REQUIRE(testMultiplexer(DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW) == DI_BIT_LOW);
    REQUIRE(testMultiplexer(DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW) == DI_BIT_HIGH);
}

TEST_CASE("Test Multiplexer Output X Ignore Y") {
    REQUIRE(testMultiplexer(DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW) == DI_BIT_HIGH);
    REQUIRE(testMultiplexer(DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_LOW) == DI_BIT_LOW);
    REQUIRE(testMultiplexer(DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_LOW) == DI_BIT_HIGH);
}

TEST_CASE("Test Multiplexer Output Y Toggle") {
    REQUIRE(testMultiplexer(DI_BIT_LOW, DI_BIT_LOW, DI_BIT_HIGH) == DI_BIT_LOW);
    REQUIRE(testMultiplexer(DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_HIGH) == DI_BIT_HIGH);
}

TEST_CASE("Test Multiplexer Output Y Ignore X") {
    REQUIRE(testMultiplexer(DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_HIGH) == DI_BIT_HIGH);
    REQUIRE(testMultiplexer(DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_HIGH) == DI_BIT_LOW);
    REQUIRE(testMultiplexer(DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH) == DI_BIT_HIGH);
}
