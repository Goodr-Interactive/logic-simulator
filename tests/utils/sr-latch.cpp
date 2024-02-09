#include <utils/sr-latch.h>

void SrLatch::addInputs(DiSimulation *simulation) {
    di_input_emit(&set, simulation);
    di_input_emit(&reset, simulation);
}

SrLatch::SrLatch() {
    di_input_init(&set, 1);
    di_input_init(&reset, 1);

    di_output_init(&value, 1);

    di_or_init(&first, 1);
    di_or_init(&second, 1);

    di_not_init(&firstNegate, 1);
    di_not_init(&secondNegate, 1);

    di_node_init(&setToFirst);
    di_node_init(&resetToSecond);
    di_node_init(&firstToFirstNegate);
    di_node_init(&secondToSecondNegate);
    di_node_init(&firstNegateToSecond);
    di_node_init(&secondNegateToFirst);

    di_connect(&setToFirst, &set.output);
    di_connect(&setToFirst, &first.input_a);

    di_connect(&resetToSecond, &reset.output);
    di_connect(&resetToSecond, &second.input_b);

    di_connect(&firstToFirstNegate, &first.output);
    di_connect(&firstToFirstNegate, &firstNegate.input);

    di_connect(&secondToSecondNegate, &second.output);
    di_connect(&secondToSecondNegate, &secondNegate.input);

    di_connect(&firstNegateToSecond, &firstNegate.output);
    di_connect(&firstNegateToSecond, &second.input_a);

    di_connect(&secondNegateToFirst, &secondNegate.output);
    di_connect(&secondNegateToFirst, &first.input_b);
}

SrLatch::~SrLatch() {
    di_node_destroy(&setToFirst);
    di_node_destroy(&resetToSecond);
    di_node_destroy(&firstToFirstNegate);
    di_node_destroy(&secondToSecondNegate);
    di_node_destroy(&firstNegateToSecond);
    di_node_destroy(&secondNegateToFirst);

    di_or_destroy(&first);
    di_or_destroy(&second);

    di_not_destroy(&firstNegate);
    di_not_destroy(&secondNegate);

    di_input_destroy(&set);
    di_input_destroy(&reset);

    di_output_destroy(&value);
}