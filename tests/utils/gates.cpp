#include <utils/gates.h>

#include <optional>

void UnaryGate::setInput(DiSimulation *simulation, DiBit value) { di_input_set_bit(&a, 0, value, simulation); }

std::optional<DiBit> UnaryGate::simulate(DiBit inputValue) {
    DiSimulation *simulation = di_simulation_create();

    std::optional<DiBit> outputValue = simulate(simulation, inputValue);

    di_simulation_free(simulation);

    return outputValue;
}

std::optional<DiBit> UnaryGate::simulate(DiSimulation *simulation, DiBit inputValue) {
    setInput(simulation, inputValue);

    bool dead = di_simulation_run(simulation, 10);

    if (dead) {
        return std::nullopt;
    }

    return di_output_get_bit(&f, 0);
}

UnaryGate::UnaryGate(DiTerminal *input, DiTerminal *output) {
    di_input_init(&a, 1);
    di_output_init(&f, 1);

    di_node_init(&aToGate, 1);
    di_node_init(&gateToF, 1);

    di_connect(&aToGate, &a.output);
    di_connect(&aToGate, input);

    di_connect(&gateToF, output);
    di_connect(&gateToF, &f.input);
}

UnaryGate::~UnaryGate() {
    di_node_destroy(&aToGate);
    di_node_destroy(&gateToF);

    di_input_destroy(&a);
    di_output_destroy(&f);
}

void BinaryGate::setInputs(DiSimulation *simulation, DiBit aValue, DiBit bValue) {
    di_input_set_bit(&a, 0, aValue, simulation);
    di_input_set_bit(&b, 0, bValue, simulation);
}

std::optional<DiBit> BinaryGate::simulate(DiBit aValue, DiBit bValue) {
    DiSimulation *simulation = di_simulation_create();

    std::optional<DiBit> outputValue = simulate(simulation, aValue, bValue);

    di_simulation_free(simulation);

    return outputValue;
}

std::optional<DiBit> BinaryGate::simulate(DiSimulation *simulation, DiBit aValue, DiBit bValue) {
    setInputs(simulation, aValue, bValue);

    bool dead = di_simulation_run(simulation, 10);

    if (dead) {
        return std::nullopt;
    }

    return di_output_get_bit(&f, 0);
}

BinaryGate::BinaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *output) {
    di_input_init(&a, 1);
    di_input_init(&b, 1);
    di_output_init(&f, 1);

    di_node_init(&aToGate, 1);
    di_node_init(&bToGate, 1);
    di_node_init(&gateToF, 1);

    di_connect(&aToGate, &a.output);
    di_connect(&aToGate, first);

    di_connect(&bToGate, &b.output);
    di_connect(&bToGate, second);

    di_connect(&gateToF, output);
    di_connect(&gateToF, &f.input);
}

BinaryGate::~BinaryGate() {
    di_node_destroy(&aToGate);
    di_node_destroy(&bToGate);
    di_node_destroy(&gateToF);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_output_destroy(&f);
}

void TertiaryGate::setInputs(DiSimulation *simulation, DiBit aValue, DiBit bValue, DiBit cValue) {
    di_input_set_bit(&a, 0, aValue, simulation);
    di_input_set_bit(&b, 0, bValue, simulation);
    di_input_set_bit(&c, 0, cValue, simulation);
}

std::optional<DiBit> TertiaryGate::simulate(DiBit aValue, DiBit bValue, DiBit cValue) {
    DiSimulation *simulation = di_simulation_create();

    std::optional<DiBit> outputValue = simulate(simulation, aValue, bValue, cValue);

    di_simulation_free(simulation);

    return outputValue;
}

std::optional<DiBit> TertiaryGate::simulate(DiSimulation *simulation, DiBit aValue, DiBit bValue, DiBit cValue) {
    setInputs(simulation, aValue, bValue, cValue);

    bool dead = di_simulation_run(simulation, 10);

    if (dead) {
        return std::nullopt;
    }

    return di_output_get_bit(&f, 0);
}

TertiaryGate::TertiaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *third, DiTerminal *output) {
    di_input_init(&a, 1);
    di_input_init(&b, 1);
    di_input_init(&c, 1);

    di_node_init(&aToGate, 1);
    di_node_init(&bToGate, 1);
    di_node_init(&cToGate, 1);
    di_node_init(&gateToF, 1);

    di_output_init(&f, 1);

    di_connect(&aToGate, &a.output);
    di_connect(&aToGate, first);

    di_connect(&bToGate, &b.output);
    di_connect(&bToGate, second);

    di_connect(&cToGate, &c.output);
    di_connect(&cToGate, third);

    di_connect(&gateToF, &f.input);
    di_connect(&gateToF, output);
}

TertiaryGate::~TertiaryGate() {
    di_node_destroy(&aToGate);
    di_node_destroy(&bToGate);
    di_node_destroy(&cToGate);
    di_node_destroy(&gateToF);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_input_destroy(&c);

    di_output_destroy(&f);
}
