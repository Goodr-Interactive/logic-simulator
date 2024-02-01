add_executable(digisim-tests
    test_signal.cpp
    test_multiplexer.cpp
    test_gates.cpp)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

target_link_libraries(digisim-tests PUBLIC digisim Catch2::Catch2WithMain)
