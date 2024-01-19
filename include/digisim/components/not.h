#pragma once

#include <digisim/component.h>
#include <digisim/connection.h>

typedef struct di_not_t {
    DiComponent component;

    size_t bits;

    DiConnection input;
    DiConnection output;
} DiNot;

void di_not_init(DiNot *self, size_t bits);
void di_not_destroy(DiNot *self);
