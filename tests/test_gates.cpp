#include <catch2/catch_all.hpp>

#include <utils/gates.h>

void testUnaryGate(DiTerminal *input, DiTerminal *output, DiBit inputValue, DiBit expected) {
    UnaryGate gate(input, output);
    std::optional<DiBit> value = gate.simulate(inputValue);

    REQUIRE(value.has_value());
    REQUIRE(*value == expected);
}

void testBinaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *output, DiBit firstValue, DiBit secondValue,
                    DiBit expected) {
    BinaryGate gate(first, second, output);
    std::optional<DiBit> value = gate.simulate(firstValue, secondValue);

    REQUIRE(value.has_value());
    REQUIRE(*value == expected);
}

TEST_CASE("Test AND Gate Low") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_AND, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_gate_destroy(&gate);
}

TEST_CASE("Test AND Gate Left High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_AND, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW);

    di_gate_destroy(&gate);
}

TEST_CASE("Test AND Right High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_AND, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_LOW);

    di_gate_destroy(&gate);
}

TEST_CASE("Test AND Gate High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_AND, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH);

    di_gate_destroy(&gate);
}

TEST_CASE("Test OR Gate Low") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_OR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_gate_destroy(&gate);
}

TEST_CASE("Test OR Gate Left High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_OR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_HIGH);

    di_gate_destroy(&gate);
}

TEST_CASE("Test OR Gate Right High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_OR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_HIGH);

    di_gate_destroy(&gate);
}

TEST_CASE("Test OR Gate High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_OR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH);

    di_gate_destroy(&gate);
}

TEST_CASE("Test XOR Gate Low") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_XOR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_gate_destroy(&gate);
}

TEST_CASE("Test XOR Gate Left High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_XOR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_HIGH);

    di_gate_destroy(&gate);
}

TEST_CASE("Test XOR Gate Right High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_XOR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_HIGH);

    di_gate_destroy(&gate);
}

TEST_CASE("Test XOR Gate High") {
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_XOR, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    testBinaryGate(input_a, input_b, &gate.output, DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_LOW);

    di_gate_destroy(&gate);
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
    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_AND, 1, 2);
    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    {
        BinaryGate binary(input_a, input_b, &gate.output);

        REQUIRE(binary.simulate(DI_BIT_LOW, DI_BIT_LOW) == DI_BIT_LOW);
        REQUIRE(binary.simulate(DI_BIT_HIGH, DI_BIT_HIGH) == DI_BIT_HIGH);
    }

    di_gate_destroy(&gate);
}
