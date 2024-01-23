#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/wire.h>
#include <digisim/component.h>
#include <digisim/components/all.h>
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

    DiWire aToAnd1;
    DiWire cToAnd2;
    DiWire bToGates;
    DiWire flipToAnd2;
    DiWire and1ToSum;
    DiWire and2ToSum;
    DiWire sumToF;

    di_input_init(&a, 1);
    di_input_init(&b, 1);
    di_input_init(&c, 1);
    di_and_init(&and1, 1);
    di_and_init(&and2, 1);
    di_not_init(&flip, 1);
    di_or_init(&sum, 1);
    di_output_init(&f, 1);

    di_wire_init(&aToAnd1);
    di_wire_init(&cToAnd2);
    di_wire_init(&bToGates);
    di_wire_init(&flipToAnd2);
    di_wire_init(&and1ToSum);
    di_wire_init(&and2ToSum);
    di_wire_init(&sumToF);

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

    di_wire_destroy(&aToAnd1);
    di_wire_destroy(&cToAnd2);
    di_wire_destroy(&bToGates);
    di_wire_destroy(&flipToAnd2);
    di_wire_destroy(&and1ToSum);
    di_wire_destroy(&and2ToSum);
    di_wire_destroy(&sumToF);
}
