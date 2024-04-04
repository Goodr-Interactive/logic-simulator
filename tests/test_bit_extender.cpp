#include <catch2/catch_test_macros.hpp>

#include <utils/bit_string.h>
#include <utils/extender.h>

TEST_CASE("Test No Extension") {
    Extender extender(DI_BIT_EXTENDER_POLICY_ONE, 5, 5);

    DiSignal in;
    di_signal_init(&in, 5);
    bit_string::copy_bit_string_to_signal(&in, "01001");

    auto result = bit_string::signal_to_bit_string(extender.simulate(&in));

    REQUIRE(result == "01001");
}

TEST_CASE("Test Zero Extension") {
    Extender extender(DI_BIT_EXTENDER_POLICY_ZERO, 5, 10);

    DiSignal in;
    di_signal_init(&in, 5);
    bit_string::copy_bit_string_to_signal(&in, "01001");

    auto result = bit_string::signal_to_bit_string(extender.simulate(&in));

    REQUIRE(result == "0000001001");
}

TEST_CASE("Test One Extension") {
    Extender extender(DI_BIT_EXTENDER_POLICY_ONE, 5, 10);

    DiSignal in;
    di_signal_init(&in, 5);
    bit_string::copy_bit_string_to_signal(&in, "01001");

    auto result = bit_string::signal_to_bit_string(extender.simulate(&in));

    REQUIRE(result == "1111101001");
}

TEST_CASE("Test Sign Extension") {
    Extender extender(DI_BIT_EXTENDER_POLICY_SIGN, 5, 10);

    DiSignal in;
    di_signal_init(&in, 5);
    bit_string::copy_bit_string_to_signal(&in, "01001");

    auto result = bit_string::signal_to_bit_string(extender.simulate(&in));

    REQUIRE(result == "0000001001");

    di_signal_init(&in, 5);
    bit_string::copy_bit_string_to_signal(&in, "11001");

    auto result2 = bit_string::signal_to_bit_string(extender.simulate(&in));

    REQUIRE(result2 == "1111111001");
}
