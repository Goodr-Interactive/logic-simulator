cmake_minimum_required(VERSION 3.27)
project(digisim LANGUAGES C CXX)

add_subdirectory(external)

add_library(digisim
    include/digisim/element.h
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

#set_target_properties(digisim PROPERTIES C_STANDARD 11)
target_include_directories(digisim PUBLIC include)

install(TARGETS digisim)

add_subdirectory(tests)

find_package(Doxygen)

if (${DOXYGEN_FOUND})
    doxygen_add_docs(digisim-docs include src)
endif()
