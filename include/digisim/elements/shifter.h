#ifndef DIGISIM_ELEMENTS_SHIFTER_H
#define DIGISIM_ELEMENTS_SHIFTER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef enum di_shifter_op_t {
    DI_SHIFTER_OP_LSL,
    DI_SHIFTER_OP_LSR,
    DI_SHIFTER_OP_ASR,
    DI_SHIFTER_OP_ROL,
    DI_SHIFTER_OP_ROR,
} DiShifterOp;

typedef struct di_shifter_t {
    DiElement element;

    size_t bits;
    size_t shift_bits;
    DiShifterOp op;

    DiTerminal input;
    DiTerminal shift;

    DiTerminal output;
} DiShifter;

void di_shifter_init(DiShifter *shifter, size_t bits, DiShifterOp op);
void di_shifter_destroy(DiShifter *shifter);

#endif // DIGISIM_ELEMENTS_SHIFTER_H
