#include <catch2/catch_test_macros.hpp>

#include <utils/wire-fight.h>
#include <utils/bit_string.h>

void simulate_wire_fight(const std::string &in_a, const std::string &in_b, const std::string &expected_bits) {
    assert(in_a.size() == in_b.size());
    assert(in_a.size() == expected_bits.size());

    WireFight fight(in_a.size());

    bit_string::copy_bit_string_to_signal(&fight.fighter_a.signal, in_a);
    bit_string::copy_bit_string_to_signal(&fight.fighter_b.signal, in_b);

    DiSimulation *simulation = di_simulation_create();

    di_input_emit(&fight.fighter_a, simulation);
    di_input_emit(&fight.fighter_b, simulation);

    di_simulation_run(simulation, 1000);

    di_simulation_free(simulation);

    bit_string::BitString expected(expected_bits);

    // On creation, all elements should be marked with di_element_changed.
    di_element_changed(&fight.value.element, nullptr);

    REQUIRE(di_signal_equal(&fight.value.signal, &expected.signal));
}

TEST_CASE("Wire Fight Zero") {
    simulate_wire_fight("00000000", "00000000", "00000000");
}

TEST_CASE("Wire Fight One") {
    simulate_wire_fight("11111111", "11111111", "11111111");
}

TEST_CASE("Wire Fight Unknown") {
    simulate_wire_fight("UUUUUUUU", "UUUUUUUU", "UUUUUUUU");
}

TEST_CASE("Wire Fight Error") {
    simulate_wire_fight("EEEEEEEE", "EEEEEEEE", "EEEEEEEE");
}

TEST_CASE("Wire Fight Alternating") {
    simulate_wire_fight("10101010", "10101010", "10101010");
}

TEST_CASE("Wire Fight Zero Wins Over Unknown") {
    simulate_wire_fight("00UU00UU", "0000UUUU", "000000UU");
}

TEST_CASE("Wire Fight One Wins Over Unknown") {
    simulate_wire_fight("11UU11UU", "1111UUUU", "111111UU");
}

TEST_CASE("Wire Fight Error Wins Over Unknown") {
    simulate_wire_fight("EEUUEEUU", "EEEEUUUU", "EEEEEEUU");
}

TEST_CASE("Wire Fight Conflict 1/0 By Bit") {
    simulate_wire_fight("11001100", "11110000", "11EEEE00");
}

TEST_CASE("Wire Fight Error Wins Over One") {
    simulate_wire_fight("11EE11EE", "1111EEEE", "11EEEEEE");
}

TEST_CASE("Wire Fight Error Wins Over Zero") {
    simulate_wire_fight("00EE00EE", "0000EEEE", "00EEEEEE");
}
