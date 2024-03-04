#include <digisim/elements/buffer.h>

#include <digisim/node.h>

void di_buffer_changed(DiElement *component, DiSimulation *simulation) {
    DiBuffer *self = (DiBuffer *)component;

    DiSignal *in = di_terminal_read(&self->input);

    di_terminal_write(&self->output, in, simulation);
}

void di_buffer_init(DiBuffer *self, size_t bits) {
    di_element_init(&self->element);

    self->element.changed = di_buffer_changed;
    self->bits = bits;

    di_terminal_init(&self->input, &self->element, bits);
    di_terminal_init(&self->output, &self->element, bits);
}

void di_buffer_destroy(DiBuffer *self) {
    di_terminal_destroy(&self->input);
    di_terminal_destroy(&self->output);

    di_element_destroy(&self->element);
}
