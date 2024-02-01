#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct Multiplexer {
    DiInput a{};
    DiInput b{};
    DiInput c{};
    DiAnd and1{};
    DiAnd and2{};
    DiOr sum{};
    DiNot flip{};
    DiOutput f{};

    DiNode aToAnd1{};
    DiNode cToAnd2{};
    DiNode bToGates{};
    DiNode flipToAnd2{};
    DiNode and1ToSum{};
    DiNode and2ToSum{};
    DiNode sumToF{};

    void connect() {
        di_connect(&aToAnd1, &a.output);
        di_connect(&aToAnd1, &and1.input_a);

        di_connect(&cToAnd2, &c.output);
        di_connect(&cToAnd2, &and2.input_b);

        di_connect(&bToGates, &b.output);
        di_connect(&bToGates, &and1.input_b);
        di_connect(&bToGates, &flip.input);

        di_connect(&flipToAnd2, &flip.output);
        di_connect(&flipToAnd2, &and2.input_a);

        di_connect(&and1ToSum, &and1.output);
        di_connect(&and1ToSum, &sum.input_a);

        di_connect(&and2ToSum, &and2.output);
        di_connect(&and2ToSum, &sum.input_b);

        di_connect(&sumToF, &sum.output);
        di_connect(&sumToF, &f.input);
    }

    void add_inputs(DiSimulation *simulation) {
        // simulation is empty here
        di_input_emit(&a, simulation);
        di_input_emit(&b, simulation);
        di_input_emit(&c, simulation);
    }

    Multiplexer() {
        di_input_init(&a, 1);
        di_input_init(&b, 1);
        di_input_init(&c, 1);
        di_and_init(&and1, 1);
        di_and_init(&and2, 1);
        di_not_init(&flip, 1);
        di_or_init(&sum, 1);
        di_output_init(&f, 1);

        di_node_init(&aToAnd1);
        di_node_init(&cToAnd2);
        di_node_init(&bToGates);
        di_node_init(&flipToAnd2);
        di_node_init(&and1ToSum);
        di_node_init(&and2ToSum);
        di_node_init(&sumToF);

        connect();
    }

    ~Multiplexer() {
        di_node_destroy(&aToAnd1);
        di_node_destroy(&cToAnd2);
        di_node_destroy(&bToGates);
        di_node_destroy(&flipToAnd2);
        di_node_destroy(&and1ToSum);
        di_node_destroy(&and2ToSum);
        di_node_destroy(&sumToF);

        di_input_destroy(&a);
        di_input_destroy(&b);
        di_input_destroy(&c);
        di_and_destroy(&and1);
        di_and_destroy(&and2);
        di_not_destroy(&flip);
        di_or_destroy(&sum);
        di_output_destroy(&f);
    }
};

TEST_CASE("Test Multiplexer Creation") { Multiplexer multiplexer; }

TEST_CASE("Test Multiplexer Step Low") {
    Multiplexer multiplexer;

    DiSimulation simulation;

    di_simulation_init(&simulation);

    multiplexer.add_inputs(&simulation);

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
    REQUIRE(di_signal_get(&multiplexer.and1.output.signal, 0) == DI_BIT_HIGH);

    REQUIRE(multiplexer.and2.output.holding);
    REQUIRE(di_signal_get(&multiplexer.and2.output.signal, 0) == DI_BIT_LOW);

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

    di_simulation_flush(&simulation, 100);

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
