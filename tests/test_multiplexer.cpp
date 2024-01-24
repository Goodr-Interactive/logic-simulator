#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
}

TEST_CASE("Test Multiplexer Creation") {
    DiInput a;
    DiInput b;
    DiInput c;
    DiAnd and1;
    DiAnd and2;
    DiOr sum;
    DiNot flip;
    DiOutput f;

    DiNode aToAnd1;
    DiNode cToAnd2;
    DiNode bToGates;
    DiNode flipToAnd2;
    DiNode and1ToSum;
    DiNode and2ToSum;
    DiNode sumToF;

    di_input_init(&a, 1);
    di_input_init(&b, 1);
    di_input_init(&c, 1);
    di_and_init(&and1, 1);
    di_and_init(&and2, 1);
    di_not_init(&flip, 1);
    di_or_init(&sum, 1);
    di_output_init(&f, 1);

    di_node_init(&aToAnd1);
    di_node_init(&cToAnd2);
    di_node_init(&bToGates);
    di_node_init(&flipToAnd2);
    di_node_init(&and1ToSum);
    di_node_init(&and2ToSum);
    di_node_init(&sumToF);

    di_connect(&aToAnd1, &a.output);
    di_connect(&aToAnd1, &and1.input_a);

    di_connect(&cToAnd2, &c.output);
    di_connect(&cToAnd2, &and2.input_b);

    di_connect(&bToGates, &b.output);
    di_connect(&bToGates, &and1.input_b);
    di_connect(&bToGates, &flip.input);

    di_connect(&flipToAnd2, &flip.output);
    di_connect(&flipToAnd2, &and2.input_a);

    di_connect(&and1ToSum, &and1.output);
    di_connect(&and1ToSum, &sum.input_a);

    di_connect(&and2ToSum, &and2.output);
    di_connect(&and2ToSum, &sum.input_b);

    di_connect(&sumToF, &sum.output);
    di_connect(&sumToF, &f.input);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_input_destroy(&c);
    di_and_destroy(&and1);
    di_and_destroy(&and2);
    di_not_destroy(&flip);
    di_or_destroy(&sum);
    di_output_destroy(&f);

    di_node_destroy(&aToAnd1);
    di_node_destroy(&cToAnd2);
    di_node_destroy(&bToGates);
    di_node_destroy(&flipToAnd2);
    di_node_destroy(&and1ToSum);
    di_node_destroy(&and2ToSum);
    di_node_destroy(&sumToF);
}
