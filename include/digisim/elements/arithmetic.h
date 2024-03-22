#ifndef DIGISIM_ELEMENTS_ARITHMETIC_H
#define DIGISIM_ELEMENTS_ARITHMETIC_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef enum di_arithmetic_op_t {
    DI_ARITHMETIC_OP_ADD,
    DI_ARITHMETIC_OP_SUB,
    DI_ARITHMETIC_OP_MUL,
    DI_ARITHMETIC_OP_MUL_SIGNED,
    // DIV is more complicated, temporarily unsupported
} DiArithmeticOp;

typedef struct di_arithmetic_t {
    DiElement element;

    size_t bits;

    DiArithmeticOp op;

    DiTerminal in_a;
    DiTerminal in_b;

    // TODO: Implement carry bits.
    // Detecting overflow in C is fairly challenging.

    DiTerminal output;
} DiArithmetic;

void di_arithmetic_init(DiArithmetic *arithmetic, size_t bits, DiArithmeticOp op);
void di_arithmetic_destroy(DiArithmetic *arithmetic);

#endif // DIGISIM_ELEMENTS_ARITHMETIC_H
