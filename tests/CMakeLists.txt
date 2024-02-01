add_executable(digisim-tests
    test_signal.cpp
    test_multiplexer.cpp)

target_link_libraries(digisim-tests PUBLIC digisim Catch2::Catch2WithMain)
