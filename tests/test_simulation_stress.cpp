#include <catch2/catch_all.hpp>

#include <utils/gates.h>

// Pop option is provided to clear the simulation queue at each step.
// Enforce option is provided for benchmarks (REQUIRE takes a lot of time)
template <bool pop, bool enforce = true> void test_simulation(size_t times) {
    DiSimulation *simulation = di_simulation_create();

    DiGate gate;

    di_gate_init(&gate, DI_GATE_OP_AND, 1, 2);

    auto input_a = di_gate_inputs_get(&gate.inputs, 0);
    auto input_b = di_gate_inputs_get(&gate.inputs, 1);

    {
        BinaryGate binary(input_a, input_b, &gate.output);

        std::array inputs = {
            std::make_tuple(DI_BIT_LOW, DI_BIT_LOW, DI_BIT_LOW),
            std::make_tuple(DI_BIT_LOW, DI_BIT_HIGH, DI_BIT_LOW),
            std::make_tuple(DI_BIT_HIGH, DI_BIT_LOW, DI_BIT_LOW),
            std::make_tuple(DI_BIT_HIGH, DI_BIT_HIGH, DI_BIT_HIGH),
        };

        for (size_t index = 0; index < times; index++) {
            auto [a, b, result] = inputs[index % inputs.size()];

            if constexpr (pop) {
                std::optional<DiBit> value = binary.simulate(simulation, a, b);

                if constexpr (enforce) {
                    REQUIRE(value.has_value());
                    REQUIRE(value.value() == result);
                }
            } else {
                binary.setInputs(simulation, a, b);
            }
        }

        if (!pop) {
            bool dead = di_simulation_run(simulation, 10);

            REQUIRE(!dead);
        }
    }

    di_gate_destroy(&gate);

    di_simulation_free(simulation);
}

TEST_CASE("Test Simulation Once") { test_simulation<true>(1); }

TEST_CASE("Test Simulation 100 Times") { test_simulation<false>(100); }

TEST_CASE("Test Simulation 100 Times (With Pop)") { test_simulation<true>(100); }

TEST_CASE("Test Simulation 50000 Times") { test_simulation<false>(50000); }

TEST_CASE("Test Simulation 50000 Times (with Pop)") { test_simulation<true>(50000); }
