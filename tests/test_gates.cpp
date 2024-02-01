#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct UnaryGate {
    DiInput a;
    DiOutput f;

    DiNode aToGate;
    DiNode gateToF;

    DiBit simulate(DiBit inputValue) {
        DiSimulation simulation;

        di_simulation_init(&simulation);

        di_input_set_bit(&a, 0, inputValue, &simulation);

        bool dead = di_simulation_flush(&simulation, 10);

        REQUIRE(!dead);

        DiBit outputValue = di_output_get_bit(&f, 0);

        di_simulation_flush(&simulation, 10);

        di_simulation_destroy(&simulation);

        return outputValue;
    }

    UnaryGate(DiTerminal *input, DiTerminal *output) {
        di_input_init(&a, 1);
        di_output_init(&f, 1);

        di_node_init(&aToGate);
        di_node_init(&gateToF);

        di_connect(&aToGate, &a.output);
        di_connect(&aToGate, input);

        di_connect(&gateToF, output);
        di_connect(&gateToF, &f.input);
    }

    ~UnaryGate() {
        di_node_destroy(&aToGate);
        di_node_destroy(&gateToF);

        di_input_destroy(&a);
        di_output_destroy(&f);
    }
};

struct BinaryGate {
    DiInput a;
    DiInput b;
    DiOutput f;

    DiNode aToGate;
    DiNode bToGate;
    DiNode gateToF;

    DiBit simulate(DiBit aValue, DiBit bValue) {
        DiSimulation simulation;

        di_simulation_init(&simulation);

        di_input_set_bit(&a, 0, aValue, &simulation);
        di_input_set_bit(&b, 0, bValue, &simulation);

        bool dead = di_simulation_flush(&simulation, 10);

        REQUIRE(!dead);

        DiBit outputValue = di_output_get_bit(&f, 0);

        di_simulation_flush(&simulation, 10);

        di_simulation_destroy(&simulation);

        return outputValue;
    }

    BinaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *output) {
        di_input_init(&a, 1);
        di_input_init(&b, 1);
        di_output_init(&f, 1);

        di_node_init(&aToGate);
        di_node_init(&bToGate);
        di_node_init(&gateToF);

        di_connect(&aToGate, &a.output);
        di_connect(&aToGate, first);

        di_connect(&bToGate, &b.output);
        di_connect(&bToGate, second);

        di_connect(&gateToF, output);
        di_connect(&gateToF, &f.input);
    }

    ~BinaryGate() {
        di_node_destroy(&aToGate);
        di_node_destroy(&bToGate);
        di_node_destroy(&gateToF);

        di_input_destroy(&a);
        di_input_destroy(&b);
        di_output_destroy(&f);
    }
};

void testUnaryGate(DiTerminal *input, DiTerminal *output, DiBit inputValue, DiBit expected) {
    UnaryGate gate(input, output);

    REQUIRE(gate.simulate(inputValue) == expected);
}

void testBinaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *output, DiBit firstValue, DiBit secondValue, DiBit expected) {
    BinaryGate gate(first, second, output);

    REQUIRE(gate.simulate(firstValue, secondValue) == expected);
}

TEST_CASE("Test AND Gate Low") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_and_destroy(&gate);
}

TEST_CASE("Test AND Gate Left High") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW);

    di_and_destroy(&gate);
}

TEST_CASE("Test AND Right High") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_LOW);

    di_and_destroy(&gate);
}

TEST_CASE("Test AND Gate High") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH);

    di_and_destroy(&gate);
}

TEST_CASE("Test OR Gate Low") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_or_destroy(&gate);
}

TEST_CASE("Test OR Gate Left High") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_HIGH);

    di_or_destroy(&gate);
}

TEST_CASE("Test OR Gate Right High") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_HIGH);

    di_or_destroy(&gate);
}

TEST_CASE("Test OR Gate High") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(
        &gate.input_a, &gate.input_b, &gate.output,
        DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH);

    di_or_destroy(&gate);
}

TEST_CASE("Test NOT Gate Low") {
    DiNot gate;

    di_not_init(&gate, 1);

    testUnaryGate(&gate.input, &gate.output, DI_BIT_LOW, DI_BIT_HIGH);

    di_not_destroy(&gate);
}

TEST_CASE("Test NOT Gate High") {
    DiNot gate;

    di_not_init(&gate, 1);

    testUnaryGate(&gate.input, &gate.output, DI_BIT_HIGH, DI_BIT_LOW);

    di_not_destroy(&gate);
}

TEST_CASE("Test AND Gate Toggle") {
    DiAnd gate;

    di_and_init(&gate, 1);

    {
        BinaryGate binary(&gate.input_a, &gate.input_b, &gate.output);

        REQUIRE(binary.simulate(DI_BIT_LOW, DI_BIT_LOW) == DI_BIT_LOW);
        REQUIRE(binary.simulate(DI_BIT_HIGH, DI_BIT_HIGH) == DI_BIT_HIGH);
    }

    di_and_destroy(&gate);
}
