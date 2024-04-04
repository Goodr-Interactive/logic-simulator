#include <utils/extender.h>

DiSignal *Extender::simulate(DiSignal *in) {
    DiSimulation *simulation = di_simulation_create();

    di_signal_copy(&input.signal, in);
    di_input_emit(&input, simulation);

    di_simulation_run(simulation, 100);

    di_simulation_free(simulation);

    return &output.signal;
}

Extender::Extender(DiBitExtenderPolicy policy, size_t in_bits, size_t out_bits) {
    di_input_init(&input, in_bits);
    di_output_init(&output, out_bits);

    di_bit_extender_init(&extender, policy, in_bits, out_bits);

    di_node_init(&inToExtender, in_bits);
    di_node_init(&extenderToOut, out_bits);

    di_connect(&inToExtender, &input.output);
    di_connect(&inToExtender, &extender.input);

    di_connect(&extenderToOut, &output.input);
    di_connect(&extenderToOut, &extender.output);
}

Extender::~Extender() {
    di_node_destroy(&inToExtender);
    di_node_destroy(&extenderToOut);

    di_input_destroy(&input);
    di_output_destroy(&output);

    di_bit_extender_destroy(&extender);
}