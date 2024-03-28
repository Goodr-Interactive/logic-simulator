#include <utils/sr-latch.h>

void SrLatch::addInputs(DiSimulation *simulation) {
    di_input_emit(&set, simulation);
    di_input_emit(&reset, simulation);
}

SrLatch::SrLatch() {
    di_input_init(&set, 1);
    di_input_init(&reset, 1);

    di_output_init(&value, 1);

    di_gate_init(&first, DI_GATE_OP_OR, 1, 2);
    di_gate_init(&second, DI_GATE_OP_OR, 1, 2);

    di_not_init(&firstNegate, 1);
    di_not_init(&secondNegate, 1);

    di_node_init(&setToFirst, 1);
    di_node_init(&resetToSecond, 1);
    di_node_init(&firstToFirstNegate, 1);
    di_node_init(&secondToSecondNegate, 1);
    di_node_init(&firstNegateToSecond, 1);
    di_node_init(&secondNegateToFirst, 1);

    di_connect(&setToFirst, &set.output);
    di_connect(&setToFirst, di_gate_inputs_get(&first.inputs, 0));

    di_connect(&resetToSecond, &reset.output);
    di_connect(&resetToSecond, di_gate_inputs_get(&second.inputs, 1));

    di_connect(&firstToFirstNegate, &first.output);
    di_connect(&firstToFirstNegate, &firstNegate.input);

    di_connect(&secondToSecondNegate, &second.output);
    di_connect(&secondToSecondNegate, &secondNegate.input);

    di_connect(&firstNegateToSecond, &firstNegate.output);
    di_connect(&firstNegateToSecond, di_gate_inputs_get(&second.inputs, 0));

    di_connect(&secondNegateToFirst, &secondNegate.output);
    di_connect(&secondNegateToFirst, di_gate_inputs_get(&first.inputs, 1));
}

SrLatch::~SrLatch() {
    di_node_destroy(&setToFirst);
    di_node_destroy(&resetToSecond);
    di_node_destroy(&firstToFirstNegate);
    di_node_destroy(&secondToSecondNegate);
    di_node_destroy(&firstNegateToSecond);
    di_node_destroy(&secondNegateToFirst);

    di_gate_destroy(&first);
    di_gate_destroy(&second);

    di_not_destroy(&firstNegate);
    di_not_destroy(&secondNegate);

    di_input_destroy(&set);
    di_input_destroy(&reset);

    di_output_destroy(&value);
}