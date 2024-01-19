#pragma once

#include <digisim/component.h>
#include <digisim/connection.h>

typedef struct di_and_t {
    DiComponent component;

    size_t bits;

    DiConnection input_a;
    DiConnection input_b;

    DiConnection output;
} DiAnd;

void di_and_init(DiAnd *self, size_t bits);
void di_and_destroy(DiAnd *self);
