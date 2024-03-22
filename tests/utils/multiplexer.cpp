#include <utils/multiplexer.h>

void Multiplexer::connect() {
    di_connect(&aToAnd1, &a.output);
    di_connect(&aToAnd1, di_gate_inputs_get(&and1.inputs, 0));

    di_connect(&cToAnd2, &c.output);
    di_connect(&cToAnd2, di_gate_inputs_get(&and2.inputs, 1));

    di_connect(&bToGates, &b.output);
    di_connect(&bToGates, di_gate_inputs_get(&and1.inputs, 1));
    di_connect(&bToGates, &flip.input);

    di_connect(&flipToAnd2, &flip.output);
    di_connect(&flipToAnd2, di_gate_inputs_get(&and2.inputs, 0));

    di_connect(&and1ToSum, &and1.output);
    di_connect(&and1ToSum, di_gate_inputs_get(&sum.inputs, 0));

    di_connect(&and2ToSum, &and2.output);
    di_connect(&and2ToSum, di_gate_inputs_get(&sum.inputs, 1));

    di_connect(&sumToF, &sum.output);
    di_connect(&sumToF, &f.input);
}

void Multiplexer::addInputs(DiSimulation *simulation) {
    // simulation is empty here
    di_input_emit(&a, simulation);
    di_input_emit(&b, simulation);
    di_input_emit(&c, simulation);
}

Multiplexer::Multiplexer() {
    di_input_init(&a, 1);
    di_input_init(&b, 1);
    di_input_init(&c, 1);
    di_and_init(&and1, 1, 2);
    di_and_init(&and2, 1, 2);
    di_not_init(&flip, 1);
    di_or_init(&sum, 1, 2);
    di_output_init(&f, 1);

    di_node_init(&aToAnd1, 1);
    di_node_init(&cToAnd2, 1);
    di_node_init(&bToGates, 1);
    di_node_init(&flipToAnd2, 1);
    di_node_init(&and1ToSum, 1);
    di_node_init(&and2ToSum, 1);
    di_node_init(&sumToF, 1);

    connect();
}

Multiplexer::~Multiplexer() {
    di_node_destroy(&aToAnd1);
    di_node_destroy(&cToAnd2);
    di_node_destroy(&bToGates);
    di_node_destroy(&flipToAnd2);
    di_node_destroy(&and1ToSum);
    di_node_destroy(&and2ToSum);
    di_node_destroy(&sumToF);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_input_destroy(&c);
    di_and_destroy(&and1);
    di_and_destroy(&and2);
    di_not_destroy(&flip);
    di_or_destroy(&sum);
    di_output_destroy(&f);
}
