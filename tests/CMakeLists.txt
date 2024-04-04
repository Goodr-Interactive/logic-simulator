add_executable(digisim-tests
    include/utils/gates.h
    include/utils/multiplexer.h
    include/utils/sr-latch.h
    include/utils/astable.h
    include/utils/wire-fight.h
    include/utils/bit_string.h
    utils/gates.cpp
    utils/multiplexer.cpp
    utils/sr-latch.cpp
    utils/astable.cpp
    utils/wire-fight.cpp
    utils/bit_string.cpp

    test_signal.cpp
    test_multiplexer.cpp
    test_gates.cpp
    test_simulation_stress.cpp
    test_ordering.cpp
    test_register.cpp
    test_fighting.cpp
    test_splitter.cpp
    test_arithmetic.cpp
)

set_target_properties(digisim-tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(digisim-tests PUBLIC digisim Catch2::Catch2WithMain)
target_include_directories(digisim-tests PUBLIC include)
