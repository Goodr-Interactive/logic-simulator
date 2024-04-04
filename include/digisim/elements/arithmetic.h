/**
* @file
* @brief Arithmetic Component (Addition, Subtraction, Multiplication)
*/

#ifndef DIGISIM_ELEMENTS_ARITHMETIC_H
#define DIGISIM_ELEMENTS_ARITHMETIC_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * The operation for the arithmetic element.
 */
typedef enum di_arithmetic_op_t {
    /**
     * Adds both inputs together.
     */
    DI_ARITHMETIC_OP_ADD,
    /**
     * Subtracts the second input from the first.
     */
    DI_ARITHMETIC_OP_SUB,
    /**
     * Multiplies both inputs together (unsigned).
     */
    DI_ARITHMETIC_OP_MUL,
    /**
     * Multiplies both inputs together (signed).
     */
    DI_ARITHMETIC_OP_MUL_SIGNED,
    // DIV is more complicated, temporarily unsupported
} DiArithmeticOp;

/**
 * Arithmetic element for math.
 *
 * To initialize/destroy, use di_arithmetic_init/di_arithmetic_destroy.
 */
typedef struct di_arithmetic_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Bit width for inputs and outputs.
     */
    size_t bits;

    /**
     * The operation to perform on the inputs.
     */
    DiArithmeticOp op;

    /**
     * First input
     */
    DiTerminal in_a;

    /**
     * Second input
     */
    DiTerminal in_b;

    // TODO: Implement carry bits.
    // Detecting overflow in C is fairly challenging.

    /**
     * Output, determined by the operation `op` and both inputs.
     */
    DiTerminal output;
} DiArithmetic;

/**
 * Initialize a DiArithmetic struct.
 *
 * @memberof DiArithmetic
 * @param arithmetic Pointer to initialize
 * @param bits Bit-size for the input/output terminals
 * @param op The operation to perform on the inputs
 */
void di_arithmetic_init(DiArithmetic *arithmetic, size_t bits, DiArithmeticOp op);

/**
 * Destroy a DiArithmetic struct.
 *
 * @memberof DiArithmetic
 * @param arithmetic Pointer to destroy
 */
void di_arithmetic_destroy(DiArithmetic *arithmetic);

#endif // DIGISIM_ELEMENTS_ARITHMETIC_H
