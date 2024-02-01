add_executable(digisim-tests
    include/utils/gates.h
    utils/gates.cpp

    test_signal.cpp
    test_multiplexer.cpp
    test_gates.cpp
    test_simulation_stress.cpp)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set_target_properties(digisim-tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(digisim-tests PUBLIC digisim Catch2::Catch2WithMain)
target_include_directories(digisim-tests PUBLIC include)
