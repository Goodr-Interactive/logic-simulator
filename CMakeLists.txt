cmake_minimum_required(VERSION 3.16)
project(digisim LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CPP_STANDARD 17)

add_subdirectory(external)

add_library(digisim STATIC
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

    src/element.c
    src/node-list.c
    src/node.c
    src/components/and.c
    src/components/or.c
    src/terminal.c
    src/signal.c
    src/components/input.c
    src/components/not.c
    src/components/output.c
    include/digisim/simulation.h
    src/simulation.c
)

target_include_directories(digisim PUBLIC include)

add_subdirectory(tests)
