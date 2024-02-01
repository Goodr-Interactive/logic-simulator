#include <catch2/catch_test_macros.hpp>

extern "C" {
#include <digisim/signal.h>
}

TEST_CASE("Signal Allocation (1 Bit)") {
    DiSignal signal;

    di_signal_init(&signal, 1);

    REQUIRE(di_signal_get(&signal, 0) == DI_BIT_LOW);

    di_signal_fill(&signal, DI_BIT_ERROR);

    REQUIRE(di_signal_get(&signal, 0) == DI_BIT_ERROR);

    di_signal_destroy(&signal);
}

TEST_CASE("Signal Allocation (128 Bit)") {
    DiSignal signal;

    di_signal_init(&signal, 128);

    for (size_t a = 0; a < signal.bits; a++) {
        assert(di_signal_get(&signal, a) == DI_BIT_LOW);

        REQUIRE(di_signal_get(&signal, a) == DI_BIT_LOW);
    }

    di_signal_fill(&signal, DI_BIT_ERROR);

    for (size_t a = 0; a < signal.bits; a++) {
        assert(di_signal_get(&signal, a) == DI_BIT_ERROR);

        REQUIRE(di_signal_get(&signal, a) == DI_BIT_ERROR);
    }

    di_signal_destroy(&signal);
}

TEST_CASE("Signal Copy (413 Bit)") {
    DiSignal signal;

    di_signal_init(&signal, 413);

    auto expected = [](size_t a) {
        return a % 2 == 0 ? DI_BIT_UNKNOWN : DI_BIT_HIGH;
    };

    for (size_t a = 0; a < signal.bits; a++) {
        REQUIRE(di_signal_get(&signal, a) == DI_BIT_LOW);

        di_signal_set(&signal, a, expected(a));
    }

    DiSignal copy;

    di_signal_init_from(&copy, &signal);

    REQUIRE(copy.bits == signal.bits);
    REQUIRE(di_signal_equal(&signal, &copy));

    for (size_t a = 0; a < copy.bits; a++) {
        REQUIRE(di_signal_get(&copy, a) == expected(a));
    }

    di_signal_destroy(&signal);
}
