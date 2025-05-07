#include <digisim/elements/shifter.h>

#include <digisim/utility/signed.h>

#include <assert.h>

size_t di_shifter_shift_bit(size_t bits) {
    // 1 -> 1, 2 -> 1, 3 -> 2, 4 -> 2, 5 -> 3, ...
    size_t acc = 0;

    bits--;

    while (bits > 0) {
        bits >>= 1;
        acc++;
    }

    return acc;
}

void di_shifter_changed(DiElement *element, DiSimulation *simulation) {
    DiShifter *shifter = (DiShifter *)element;

    DiSignal *input_signal = di_terminal_read(&shifter->input);
    DiSignal *shift_signal = di_terminal_read(&shifter->shift);

    uint64_t value = di_signal_get_values(input_signal)[0] & DI_SIGNED_TOP_MASK(shifter->bits);
    uint64_t shift = di_signal_get_values(shift_signal)[0] & DI_SIGNED_TOP_MASK(shifter->shift_bits);

    uint64_t result;

    switch (shifter->op) {
    case DI_SHIFTER_OP_LSL:
        result = value << shift;
        break;
    case DI_SHIFTER_OP_LSR:
        result = value >> shift;
        break;
    case DI_SHIFTER_OP_ASR:
        if (((1 << (shifter->bits - 1)) & value) != 0) {
            if (shift > shifter->bits) {
                result = ~(uint64_t)0; // all bits 1
            } else {
                result = (value >> shift) | DI_SIGNED_TOP_MASK(shifter->bits - shift);
            }
        } else {
            result = value >> shift;
        }

        break;
    case DI_SHIFTER_OP_ROL: {
        // shift bits left shifted by the amount of bits we didn't delete
        uint64_t mask = ((~(uint64_t)0) >> (64 - shift)) << (shifter->bits - shift);

        result = ((value << shift) | ((value & mask) >> (shifter->bits - shift))) & ~DI_SIGNED_TOP_MASK(shifter->bits);

        break;
    }
    case DI_SHIFTER_OP_ROR: {
        uint64_t mask = (~(uint64_t)0) >> (64 - shift);

        result = (value >> shift) || ((value & mask) << (shifter->bits - shift));
        break;
    }
    }

    // Make sure we clear the output, e.g. set unknown and error bits to zero.
    di_signal_fill(&shifter->output.signal, DI_BIT_LOW);

    di_signal_get_values(&shifter->output.signal)[0] = result;

    di_terminal_send(&shifter->output, simulation);
}

void di_shifter_init(DiShifter *shifter, size_t bits, DiShifterOp op) {
    assert(bits <= 64);

    di_element_init(&shifter->element);

    shifter->element.changed = di_shifter_changed;

    shifter->bits = bits;
    shifter->shift_bits = di_shifter_shift_bit(bits);
    shifter->op = op;

    di_terminal_init(&shifter->input, &shifter->element, bits);
    di_terminal_init(&shifter->shift, &shifter->element, shifter->shift_bits);
    di_terminal_init(&shifter->output, &shifter->element, bits);
}

void di_shifter_destroy(DiShifter *shifter) {
    di_terminal_destroy(&shifter->input);
    di_terminal_destroy(&shifter->shift);
    di_terminal_destroy(&shifter->output);

    di_element_destroy(&shifter->element);
}
