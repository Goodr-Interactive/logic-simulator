#include <catch2/catch_all.hpp>

#include <utils/multiplexer.h>

TEST_CASE("Test Multiplexer Creation") { Multiplexer multiplexer; }

TEST_CASE("Test Multiplexer Step Low") {
    Multiplexer multiplexer;

    DiSimulation simulation;

    di_simulation_init(&simulation);

    multiplexer.addInputs(&simulation);

    REQUIRE(simulation.count == 3);

    di_simulation_step(&simulation);

    REQUIRE(multiplexer.and1.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and1.output.signal, 0) == DI_BIT_LOW);

    REQUIRE(multiplexer.and2.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and2.output.signal, 0) == DI_BIT_LOW);

    di_simulation_destroy(&simulation);
}

TEST_CASE("Test Multiplexer Step High") {
    Multiplexer multiplexer;

    DiSimulation simulation;

    di_simulation_init(&simulation);

    di_input_set_bit(&multiplexer.c, 0, DI_BIT_HIGH, &simulation);
    di_input_set_bit(&multiplexer.b, 0, DI_BIT_LOW, &simulation);
    di_input_set_bit(&multiplexer.a, 0, DI_BIT_LOW, &simulation);

    REQUIRE(simulation.count == 3);

    di_simulation_step(&simulation);
    di_simulation_step(&simulation);

    REQUIRE(multiplexer.and1.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and1.output.signal, 0) == DI_BIT_LOW);

    REQUIRE(multiplexer.and2.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and2.output.signal, 0) == DI_BIT_HIGH);

    di_simulation_destroy(&simulation);
}

DiBit testMultiplexer(DiBit x, DiBit y, DiBit s) {
    Multiplexer multiplexer;

    DiSimulation simulation;

    di_simulation_init(&simulation);

    di_input_set_bit(&multiplexer.c, 0, x, &simulation);
    di_input_set_bit(&multiplexer.b, 0, s, &simulation);
    di_input_set_bit(&multiplexer.a, 0, y, &simulation);

    REQUIRE(simulation.count == 3);

    di_simulation_run(&simulation, 100);

    DiBit value = di_signal_get(&multiplexer.f.signal, 0);

    di_simulation_destroy(&simulation);

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
