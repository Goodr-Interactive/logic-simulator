#include <catch2/catch_all.hpp>

#include <utils/sr-latch.h>

extern "C" {
#include <digisim/simulations/unit-delay.h>
}

TEST_CASE("Test SR Latch Is Astable") {
    SrLatch latch;

    DiUnitSimulation simulation;
    di_unit_simulation_init(&simulation);

    latch.addInputs(&simulation.simulation);

    bool dead = di_simulation_run(&simulation.simulation, 100);

    REQUIRE(dead);

    di_unit_simulation_destroy(&simulation);
}

TEST_CASE("Test SR Latch Ordering") {
    SrLatch latch;

    DiSimulation *simulation = di_simulation_create();

    latch.addInputs(simulation);

    di_input_set_bit(&latch.set, 0, DI_BIT_HIGH, simulation);

    bool dead = di_simulation_run(simulation, 100);

    REQUIRE(!dead);

    DiBit firstBit = di_signal_get(&latch.firstToFirstNegate.signal, 0);
    DiBit secondBit = di_signal_get(&latch.secondToSecondNegate.signal, 0);

    REQUIRE(firstBit == DI_BIT_HIGH);
    REQUIRE(secondBit == DI_BIT_LOW);

    di_simulation_free(simulation);
}
