#ifndef DIGISIM_REGISTER_H
#define DIGISIM_REGISTER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef struct di_register_t {
    DiElement element;

    size_t bits;

    bool did_send;
    bool *hold;
    bool *state;

    DiTerminal data;
    DiTerminal clock;
    DiTerminal reset;
    DiTerminal value;
} DiRegister;

void di_register_init(DiRegister *latch, size_t bits);
void di_register_destroy(DiRegister *latch);

#endif // DIGISIM_REGISTER_H
