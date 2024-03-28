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
