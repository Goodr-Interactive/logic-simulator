#pragma once

#include <digisim/component.h>
#include <digisim/connection.h>

typedef struct di_or_t {
    DiComponent component;

    size_t bits;

    DiConnection input_a;
    DiConnection input_b;

    DiConnection output;
} DiOr;

void di_or_init(DiOr *self, size_t bits);
void di_or_destroy(DiOr *self);
