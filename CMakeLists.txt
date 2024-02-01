cmake_minimum_required(VERSION 3.27)
project(digisim LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CPP_STANDARD 17)

add_subdirectory(external)

add_library(digisim
    include/digisim/element.h
    include/digisim/node-list.h
    include/digisim/node.h
    include/digisim/elements/and.h
    include/digisim/elements/or.h
    include/digisim/elements/all.h
    include/digisim/elements/input.h
    include/digisim/elements/output.h
    include/digisim/terminal.h
    include/digisim/signal.h
    include/digisim/elements/not.h
    include/digisim/simulation.h

    src/element.c
    src/node-list.c
    src/node.c
    src/elements/and.c
    src/elements/or.c
    src/terminal.c
    src/signal.c
    src/elements/input.c
    src/elements/not.c
    src/elements/output.c
    src/simulation.c
)

target_include_directories(digisim PUBLIC include)

install(TARGETS digisim)

add_subdirectory(tests)
