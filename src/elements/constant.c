#include <digisim/elements/constant.h>

#include <assert.h>

void di_constant_emit(DiConstant *constant, DiSimulation *simulation) {
    di_signal_fill(&constant->output.signal, DI_BIT_LOW);

    di_signal_get_values(&constant->output.signal)[0] = constant->value;

    di_terminal_send(&constant->output, simulation);
}

void di_constant_changed(DiElement *element, DiSimulation *simulation) {
    DiConstant *constant = (DiConstant *)element;

    // Changed needs to be called circuit wide to converge (e.g. shaken?)

    di_constant_emit(constant, simulation);
}

void di_constant_init(DiConstant *constant, size_t bits, uint64_t value) {
    assert(bits <= 64);

    di_element_init(&constant->element);

    constant->element.changed = di_constant_changed;

    constant->bits = bits;
    constant->value = value;

    di_terminal_init(&constant->output, &constant->element, bits);
}

void di_constant_destroy(DiConstant *constant) {
    di_terminal_destroy(&constant->output);

    di_element_destroy(&constant->element);
}
