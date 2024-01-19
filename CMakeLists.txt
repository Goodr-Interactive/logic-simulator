cmake_minimum_required(VERSION 3.27)
project(digisim C)

set(CMAKE_C_STANDARD 11)

add_library(digisim STATIC
    include/digisim/component.h
    include/digisim/connection-list.h
    include/digisim/wire.h
    include/digisim/components/and.h
    include/digisim/components/or.h
    include/digisim/components/all.h
    include/digisim/components/input.h
    include/digisim/components/output.h
    include/digisim/connection.h
    include/digisim/signal.h
    include/digisim/components/not.h

    src/component.c
    src/connection-list.c
    src/wire.c
    src/components/and.c
    src/components/or.c
    src/connection.c
    src/signal.c
    src/components/input.c
    src/components/not.c
    src/components/output.c
)

target_include_directories(digisim PUBLIC include)

add_executable(digisim-test main.c)

target_link_libraries(digisim-test PUBLIC digisim)
