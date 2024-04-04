cmake_minimum_required(VERSION 3.16)
project(digisim LANGUAGES C CXX)

add_subdirectory(external)

add_library(digisim
    include/digisim/node.h
    include/digisim/signal.h
    include/digisim/element.h
    include/digisim/terminal.h
    include/digisim/simulation.h
    include/digisim/simulations/unit-delay.h
    include/digisim/simulations/zero-delay.h
    include/digisim/elements/all.h
    include/digisim/elements/not.h
    include/digisim/elements/gate.h
    include/digisim/elements/input.h
    include/digisim/elements/output.h
    include/digisim/elements/buffer.h
    include/digisim/elements/d-latch.h
    include/digisim/elements/sr-latch.h
    include/digisim/elements/multiplexer.h
    include/digisim/elements/demultiplexer.h
    include/digisim/elements/register.h
    include/digisim/elements/connector.h
    include/digisim/elements/splitter.h
    include/digisim/elements/bit-extender.h
    include/digisim/utility/node-list.h
    include/digisim/utility/terminal-list.h
    include/digisim/utility/gate_inputs.h
    include/digisim/utility/print.h

    src/node.c
    src/signal.c
    src/element.c
    src/terminal.c
    src/simulation.c
    src/simulations/unit-delay.c
    src/simulations/zero-delay.c
    src/elements/gate.c
    src/elements/input.c
    src/elements/not.c
    src/elements/output.c
    src/elements/buffer.c
    src/elements/d-latch.c
    src/elements/sr-latch.c
    src/elements/multiplexer.c
    src/elements/demultiplexer.c
    src/elements/register.c
    src/elements/connector.c
    src/elements/splitter.c
    src/elements/bit-extender.c
    src/utility/node-list.c
    src/utility/terminal-list.c
    src/utility/gate_inputs.c
    src/utility/print.c
)

set_target_properties(digisim PROPERTIES C_STANDARD 11)
target_include_directories(digisim PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

install(TARGETS digisim)

add_subdirectory(tests)

find_package(Doxygen)

if (${DOXYGEN_FOUND})
    set(DOXYGEN_TYPEDEF_HIDES_STRUCT YES)

    doxygen_add_docs(digisim-docs include src)
endif()
