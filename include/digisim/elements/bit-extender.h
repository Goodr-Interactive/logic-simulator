#ifndef DIGISIM_ELEMENTS_BIT_EXTENDER_H
#define DIGISIM_ELEMENTS_BIT_EXTENDER_H

#include <digisim/element.h>
#include <digisim/terminal.h>

typedef enum di_bit_extender_policy_t {
    DI_BIT_EXTENDER_POLICY_ZERO,
    DI_BIT_EXTENDER_POLICY_ONE,
    DI_BIT_EXTENDER_POLICY_SIGN,
} DiBitExtenderPolicy;

typedef struct di_bit_extender_t {
    DiElement element;

    DiBitExtenderPolicy policy;

    size_t in_bits;
    size_t out_bits;

    DiTerminal input;
    DiTerminal output;
} DiBitExtender;

void di_bit_extender_init(DiBitExtender *extender, DiBitExtenderPolicy policy, size_t in_bits, size_t out_bits);
void di_bit_extender_destroy(DiBitExtender *extender);

#endif // DIGISIM_ELEMENTS_BIT_EXTENDER_H
