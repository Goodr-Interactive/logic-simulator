#include <catch2/catch_all.hpp>

#include <utils/gates.h>

void testUnaryGate(DiTerminal *input, DiTerminal *output, DiBit inputValue, DiBit expected) {
    UnaryGate gate(input, output);

    REQUIRE(gate.simulate(inputValue) == expected);
}

void testBinaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *output, DiBit firstValue, DiBit secondValue,
                    DiBit expected) {
    BinaryGate gate(first, second, output);

    REQUIRE(gate.simulate(firstValue, secondValue) == expected);
}

TEST_CASE("Test AND Gate Low") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_and_destroy(&gate);
}

TEST_CASE("Test AND Gate Left High") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW);

    di_and_destroy(&gate);
}

TEST_CASE("Test AND Right High") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_LOW);

    di_and_destroy(&gate);
}

TEST_CASE("Test AND Gate High") {
    DiAnd gate;

    di_and_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH);

    di_and_destroy(&gate);
}

TEST_CASE("Test OR Gate Low") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW);

    di_or_destroy(&gate);
}

TEST_CASE("Test OR Gate Left High") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_HIGH);

    di_or_destroy(&gate);
}

TEST_CASE("Test OR Gate Right High") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_HIGH);

    di_or_destroy(&gate);
}

TEST_CASE("Test OR Gate High") {
    DiOr gate;

    di_or_init(&gate, 1);

    testBinaryGate(&gate.input_a, &gate.input_b, &gate.output, DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH);

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
