#include <catch2/catch_test_macros.hpp>

#include <utils/wire-fight.h>
#include <utils/bit_string.h>

TEST_CASE("Test Splitter Passthrough End") {
    DiSplitter splitter;

    size_t splits[1] = { 1 };
    di_splitter_init(&splitter, 1, 1, splits);

    DiInput in;
    DiNode in_node;
    DiOutput out;
    DiNode out_node;

    di_input_init(&in, 1);
    di_node_init(&in_node, 1);
    di_output_init(&out, 1);
    di_node_init(&out_node, 1);

    di_connect(&in_node, &in.output);
    di_connect(&in_node, &splitter.splits[0]); // input through split

    di_connect(&out_node, &out.input);
    di_connect(&out_node, &splitter.end); // input through split

    DiSimulation *simulation = di_simulation_create();

    di_input_set_bit(&in, 0, DI_BIT_HIGH, simulation);
    REQUIRE(!di_simulation_run(simulation, 100));

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_HIGH);

    di_input_set_bit(&in, 0, DI_BIT_LOW, simulation);
    REQUIRE(!di_simulation_run(simulation, 100));

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_LOW);

    di_simulation_free(simulation);

    di_node_destroy(&in_node);
    di_node_destroy(&out_node);

    di_input_destroy(&in);
    di_output_destroy(&out);

    di_splitter_destroy(&splitter);
}

TEST_CASE("Test Splitter Passthrough Split") {
    DiSplitter splitter;

    size_t splits[1] = { 1 };
    di_splitter_init(&splitter, 1, 1, splits);

    DiInput in;
    DiNode in_node;
    DiOutput out;
    DiNode out_node;

    di_input_init(&in, 1);
    di_node_init(&in_node, 1);
    di_output_init(&out, 1);
    di_node_init(&out_node, 1);

    di_connect(&in_node, &in.output);
    di_connect(&in_node, &splitter.end); // input through split

    di_connect(&out_node, &out.input);
    di_connect(&out_node, &splitter.splits[0]); // input through split

    DiSimulation *simulation = di_simulation_create();

    di_input_set_bit(&in, 0, DI_BIT_HIGH, simulation);
    di_simulation_run(simulation, 100);

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_HIGH);

    di_input_set_bit(&in, 0, DI_BIT_LOW, simulation);
    di_simulation_run(simulation, 100);

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_LOW);

    di_simulation_free(simulation);

    di_node_destroy(&in_node);
    di_node_destroy(&out_node);

    di_input_destroy(&in);
    di_output_destroy(&out);

    di_splitter_destroy(&splitter);
}

TEST_CASE("Test Splitter Merge 2") {
    DiSplitter splitter;

    size_t splits[2] = { 1, 1 };
    di_splitter_init(&splitter, 2, 2, splits);

    DiInput in_1;
    DiNode in_1_node;
    DiInput in_2;
    DiNode in_2_node;
    DiOutput out;
    DiNode out_node;

    di_input_init(&in_1, 1);
    di_node_init(&in_1_node, 1);
    di_input_init(&in_2, 1);
    di_node_init(&in_2_node, 1);
    di_output_init(&out, 2);
    di_node_init(&out_node, 2);

    di_connect(&in_1_node, &in_1.output);
    di_connect(&in_1_node, &splitter.splits[0]);

    di_connect(&in_2_node, &in_2.output);
    di_connect(&in_2_node, &splitter.splits[1]);

    di_connect(&out_node, &out.input);
    di_connect(&out_node, &splitter.end);

    DiSimulation *simulation = di_simulation_create();

    // Both high
    di_input_set_bit(&in_1, 0, DI_BIT_HIGH, simulation);
    di_input_set_bit(&in_2, 0, DI_BIT_HIGH, simulation);
    REQUIRE(!di_simulation_run(simulation, 100));

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_HIGH);
    REQUIRE(di_output_get_bit(&out, 1) == DI_BIT_HIGH);

    // Both Low
    di_input_set_bit(&in_1, 0, DI_BIT_LOW, simulation);
    di_input_set_bit(&in_2, 0, DI_BIT_LOW, simulation);
    REQUIRE(!di_simulation_run(simulation, 100));

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_LOW);
    REQUIRE(di_output_get_bit(&out, 1) == DI_BIT_LOW);

    // One High
    di_input_set_bit(&in_1, 0, DI_BIT_LOW, simulation);
    di_input_set_bit(&in_2, 0, DI_BIT_HIGH, simulation);
    REQUIRE(!di_simulation_run(simulation, 100));

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_LOW);
    REQUIRE(di_output_get_bit(&out, 1) == DI_BIT_HIGH);

    di_input_set_bit(&in_1, 0, DI_BIT_HIGH, simulation);
    di_input_set_bit(&in_2, 0, DI_BIT_LOW, simulation);
    REQUIRE(!di_simulation_run(simulation, 100));

    REQUIRE(di_output_get_bit(&out, 0) == DI_BIT_HIGH);
    REQUIRE(di_output_get_bit(&out, 1) == DI_BIT_LOW);

    di_simulation_free(simulation);

    di_node_destroy(&in_1_node);
    di_node_destroy(&in_2_node);
    di_node_destroy(&out_node);

    di_input_destroy(&in_1);
    di_input_destroy(&in_2);
    di_output_destroy(&out);

    di_splitter_destroy(&splitter);
}

TEST_CASE("Test Splitter Split 2") {
    DiSplitter splitter;

    size_t splits[2] = { 1, 1 };
    di_splitter_init(&splitter, 2, 2, splits);

    DiInput in;
    DiNode in_node;
    DiOutput out_1;
    DiNode out_1_node;
    DiOutput out_2;
    DiNode out_2_node;

    di_input_init(&in, 2);
    di_node_init(&in_node, 2);
    di_output_init(&out_1, 1);
    di_node_init(&out_1_node, 1);
    di_output_init(&out_2, 1);
    di_node_init(&out_2_node, 1);

    di_connect(&in_node, &in.output);
    di_connect(&in_node, &splitter.end);

    di_connect(&out_1_node, &out_1.input);
    di_connect(&out_1_node, &splitter.splits[0]);

    di_connect(&out_2_node, &out_2.input);
    di_connect(&out_2_node, &splitter.splits[1]);

    DiSimulation *simulation = di_simulation_create();

    di_input_set_bit(&in, 0, DI_BIT_HIGH, simulation);
    di_input_set_bit(&in, 1, DI_BIT_HIGH, simulation);
    di_simulation_run(simulation, 100);
    REQUIRE(di_output_get_bit(&out_1, 0) == DI_BIT_HIGH);
    REQUIRE(di_output_get_bit(&out_2, 0) == DI_BIT_HIGH);

    di_input_set_bit(&in, 0, DI_BIT_LOW, simulation);
    di_input_set_bit(&in, 1, DI_BIT_LOW, simulation);
    di_simulation_run(simulation, 100);
    REQUIRE(di_output_get_bit(&out_1, 0) == DI_BIT_LOW);
    REQUIRE(di_output_get_bit(&out_2, 0) == DI_BIT_LOW);

    di_input_set_bit(&in, 0, DI_BIT_HIGH, simulation);
    di_input_set_bit(&in, 1, DI_BIT_LOW, simulation);
    di_simulation_run(simulation, 100);
    REQUIRE(di_output_get_bit(&out_1, 0) == DI_BIT_HIGH);
    REQUIRE(di_output_get_bit(&out_2, 0) == DI_BIT_LOW);

    di_input_set_bit(&in, 0, DI_BIT_LOW, simulation);
    di_input_set_bit(&in, 1, DI_BIT_HIGH, simulation);
    di_simulation_run(simulation, 100);
    REQUIRE(di_output_get_bit(&out_1, 0) == DI_BIT_LOW);
    REQUIRE(di_output_get_bit(&out_2, 0) == DI_BIT_HIGH);

    di_simulation_free(simulation);

    di_node_destroy(&in_node);
    di_node_destroy(&out_1_node);
    di_node_destroy(&out_2_node);

    di_input_destroy(&in);
    di_output_destroy(&out_1);
    di_output_destroy(&out_2);

    di_splitter_destroy(&splitter);
}

TEST_CASE("Test Splitter Feedback") {
    // Seems like splitters can act as a feedback loop, causing their values to never change.
    DiInput i;
    DiOutput o;

    DiSplitter in_splitter;
    DiSplitter out_splitter;

    DiNode iToIn;
    DiNode in_to_out_1;
    DiNode in_to_out_2;
    DiNode outToO;

    di_input_init(&i, 2);
    di_output_init(&o, 2);

    size_t splits[2] = { 1, 1 };

    di_splitter_init(&in_splitter, 2, 2, splits);
    di_splitter_init(&out_splitter, 2, 2, splits);

    di_node_init(&iToIn, 2);
    di_node_init(&in_to_out_1, 1);
    di_node_init(&in_to_out_2, 1);
    di_node_init(&outToO, 2);

    di_connect(&iToIn, &i.output);
    di_connect(&iToIn, &in_splitter.end);

    di_connect(&in_to_out_1, &in_splitter.splits[0]);
    di_connect(&in_to_out_1, &out_splitter.splits[0]);

    di_connect(&in_to_out_2, &in_splitter.splits[1]);
    di_connect(&in_to_out_2, &out_splitter.splits[1]);

    di_connect(&outToO, &out_splitter.end);
    di_connect(&outToO, &o.input);

    DiSimulation *simulation = di_simulation_create();

    // These tests are meant to be done in sequence.
    di_input_set_bit(&i, 0, DI_BIT_LOW, simulation);
    di_input_set_bit(&i, 1, DI_BIT_LOW, simulation);

    bool dead;

    dead = di_simulation_run(simulation, 5);

    REQUIRE(!dead);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_LOW);
    REQUIRE(di_output_get_bit(&o, 1) == DI_BIT_LOW);

    di_input_set_bit(&i, 0, DI_BIT_HIGH, simulation);
    di_input_set_bit(&i, 1, DI_BIT_HIGH, simulation);

    dead = di_simulation_run(simulation, 20);

    REQUIRE(!dead);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_HIGH);
    REQUIRE(di_output_get_bit(&o, 1) == DI_BIT_HIGH);

    di_simulation_free(simulation);

    di_node_destroy(&iToIn);
    di_node_destroy(&in_to_out_1);
    di_node_destroy(&in_to_out_2);
    di_node_destroy(&outToO);

    di_splitter_destroy(&in_splitter);
    di_splitter_destroy(&out_splitter);

    di_input_destroy(&i);
    di_output_destroy(&o);
}

TEST_CASE("Test Connector Passthrough") {
    DiConnector connector;

    DiInput i;
    DiOutput o;

    DiNode iToCon;
    DiNode conToO;

    di_connector_init(&connector, 1);

    di_input_init(&i, 1);
    di_output_init(&o, 1);

    di_node_init(&iToCon, 1);
    di_node_init(&conToO, 1);

    di_connect(&iToCon, &i.output);
    di_connect(&iToCon, &connector.connection_a);

    di_connect(&conToO, &o.input);
    di_connect(&conToO, &connector.connection_b);

    DiSimulation *simulation = di_simulation_create();

    di_input_set_bit(&i, 0, DI_BIT_LOW, simulation);

    di_simulation_run(simulation, 100);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_LOW);

    di_input_set_bit(&i, 0, DI_BIT_HIGH, simulation);

    di_simulation_run(simulation, 100);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_HIGH);

    di_simulation_free(simulation);

    di_node_destroy(&iToCon);
    di_node_destroy(&conToO);

    di_connector_destroy(&connector);

    di_input_destroy(&i);
    di_output_destroy(&o);
}

TEST_CASE("Test Connector Feedback") {
    DiConnector connector1;
    DiConnector connector2;

    DiInput i;
    DiOutput o;

    DiNode con1ToCon2;

    DiNode iToCon;
    DiNode conToO;

    di_connector_init(&connector1, 1);
    di_connector_init(&connector2, 1);

    di_input_init(&i, 1);
    di_output_init(&o, 1);

    di_node_init(&iToCon, 1);
    di_node_init(&conToO, 1);
    di_node_init(&con1ToCon2, 1);

    di_connect(&iToCon, &i.output);
    di_connect(&iToCon, &connector1.connection_a);

    di_connect(&conToO, &o.input);
    di_connect(&conToO, &connector2.connection_b);

    di_connect(&con1ToCon2, &connector1.connection_b);
    di_connect(&con1ToCon2, &connector2.connection_a);

    DiSimulation *simulation = di_simulation_create();

    di_input_set_bit(&i, 0, DI_BIT_LOW, simulation);

    bool dead;

    dead = di_simulation_run(simulation, 100);

    REQUIRE(!dead);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_LOW);

    di_input_set_bit(&i, 0, DI_BIT_HIGH, simulation);

    dead = di_simulation_run(simulation, 100);

    REQUIRE(!dead);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_HIGH);

    di_input_set_bit(&i, 0, DI_BIT_LOW, simulation);

    dead = di_simulation_run(simulation, 100);

    REQUIRE(!dead);

    REQUIRE(di_output_get_bit(&o, 0) == DI_BIT_LOW);

    di_simulation_free(simulation);

    di_node_destroy(&iToCon);
    di_node_destroy(&conToO);
    di_node_destroy(&con1ToCon2);

    di_connector_destroy(&connector1);
    di_connector_destroy(&connector2);

    di_input_destroy(&i);
    di_output_destroy(&o);
}
