#include <digisim/elements/bit-extender.h>

void di_bit_extender_changed(DiElement *element, DiSimulation *simulation) {
    DiBitExtender *extender = (DiBitExtender *)element;

    DiSignal *input = di_terminal_read(&extender->input);

    DiBit last = DI_BIT_LOW;

    for (size_t a = 0; a < extender->out_bits; a++) {
        DiBit bit;

        if (a < extender->in_bits) {
            last = di_signal_get(input, a);

            bit = last;
        } else {
            switch (extender->policy) {
            case DI_BIT_EXTENDER_POLICY_ZERO:
                bit = DI_BIT_LOW;
                break;

            case DI_BIT_EXTENDER_POLICY_ONE:
                bit = DI_BIT_HIGH;
                break;

            case DI_BIT_EXTENDER_POLICY_SIGN:
                bit = last;
                break;
            }
        }

        di_signal_set(&extender->output.signal, a, bit);
    }

    di_terminal_send(&extender->output, simulation);
}

void di_bit_extender_init(DiBitExtender *extender, DiBitExtenderPolicy policy, size_t in_bits, size_t out_bits) {
    di_element_init(&extender->element);

    extender->element.changed = di_bit_extender_changed;

    extender->policy = policy;

    extender->in_bits = in_bits;
    extender->out_bits = out_bits;

    di_terminal_init(&extender->input, &extender->element, in_bits);
    di_terminal_init(&extender->output, &extender->element, out_bits);
}

void di_bit_extender_destroy(DiBitExtender *extender) {
    di_terminal_destroy(&extender->input);
    di_terminal_destroy(&extender->output);

    di_element_destroy(&extender->element);
}
