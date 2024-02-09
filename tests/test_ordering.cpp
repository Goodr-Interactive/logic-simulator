#include <catch2/catch_all.hpp>

#include <utils/sr-latch.h>

TEST_CASE("Test SR Latch Is Astable") {
    SrLatch latch;

    DiSimulation simulation;

    di_simulation_init(&simulation);

    latch.addInputs(&simulation);

    bool dead = di_simulation_run(&simulation, 100);

    REQUIRE(dead);

    di_simulation_destroy(&simulation);
}

TEST_CASE("Test SR Latch Ordering") {
    SrLatch latch;

    DiSimulation simulation;

    di_simulation_init(&simulation);

    latch.addInputs(&simulation);

    di_input_set_bit(&latch.set, 0, DI_BIT_HIGH, &simulation);

    bool dead = di_simulation_run(&simulation, 100);

    REQUIRE(!dead);

    REQUIRE(latch.firstToFirstNegate.has_signal);
    REQUIRE(latch.secondToSecondNegate.has_signal);

    DiBit firstBit = di_signal_get(&latch.firstToFirstNegate.signal, 0);
    DiBit secondBit = di_signal_get(&latch.secondToSecondNegate.signal, 0);

    REQUIRE(firstBit == DI_BIT_HIGH);
    REQUIRE(secondBit == DI_BIT_LOW);

    di_simulation_destroy(&simulation);
}
