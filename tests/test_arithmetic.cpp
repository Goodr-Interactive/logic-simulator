#include <catch2/catch_all.hpp>

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

#include <utils/bit_string.h>

TEST_CASE("Test Constant Has Value") {
    DiOutput out;
    DiNode valueToOut;
    DiConstant value;

    di_output_init(&out, 16);
    di_node_init(&valueToOut, 16);
    di_constant_init(&value, 16, 0x3FC7);

    di_connect(&valueToOut, &out.input);
    di_connect(&valueToOut, &value.output);

    DiSimulation *simulation = di_simulation_create();

    // Alternatively, shake the circuit by making every element fire a "changed" event.
    di_constant_emit(&value, simulation);

    di_simulation_run(simulation, 100);

    REQUIRE((di_signal_get_values(&out.signal)[0] & 0xFFFF) == 0x3FC7);

    di_simulation_free(simulation);

    di_node_destroy(&valueToOut);

    di_output_destroy(&out);
    di_constant_destroy(&value);
}

TEST_CASE("Test Arithmetic Add") {
    DiInput a;
    DiInput b;

    DiArithmetic math;

    DiOutput o;

    DiNode aToMath;
    DiNode bToMath;

    DiNode mathToO;

    di_input_init(&a, 8);
    di_input_init(&b, 8);
    di_arithmetic_init(&math, 8, DI_ARITHMETIC_OP_ADD);
    di_output_init(&o, 8);

    di_node_init(&aToMath, 8);
    di_node_init(&bToMath, 8);
    di_node_init(&mathToO, 8);

    di_connect(&aToMath, &a.output);
    di_connect(&aToMath, &math.in_a);

    di_connect(&bToMath, &b.output);
    di_connect(&bToMath, &math.in_b);

    di_connect(&mathToO, &math.output);
    di_connect(&mathToO, &o.input);

    DiSimulation *simulation = di_simulation_create();

    DiSignal signal1;
    DiSignal signal2;

    di_signal_init(&signal1, 8);
    di_signal_init(&signal2, 8);

    bit_string::copy_bit_string_to_signal(&signal1, "01101111");
    bit_string::copy_bit_string_to_signal(&signal2, "00010001");

    di_input_set(&a, signal1, simulation);
    di_input_set(&b, signal2, simulation);

    di_simulation_run(simulation, 100);

    auto value = bit_string::signal_to_bit_string(&o.signal);

    REQUIRE(value == "10000000");

    di_simulation_free(simulation);

    di_node_destroy(&aToMath);
    di_node_destroy(&bToMath);
    di_node_destroy(&mathToO);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_arithmetic_destroy(&math);
    di_output_destroy(&o);
}

TEST_CASE("Test Arithmetic Sub") {
    DiInput a;
    DiInput b;

    DiArithmetic math;

    DiOutput o;

    DiNode aToMath;
    DiNode bToMath;

    DiNode mathToO;

    di_input_init(&a, 8);
    di_input_init(&b, 8);
    di_arithmetic_init(&math, 8, DI_ARITHMETIC_OP_SUB);
    di_output_init(&o, 8);

    di_node_init(&aToMath, 8);
    di_node_init(&bToMath, 8);
    di_node_init(&mathToO, 8);

    di_connect(&aToMath, &a.output);
    di_connect(&aToMath, &math.in_a);

    di_connect(&bToMath, &b.output);
    di_connect(&bToMath, &math.in_b);

    di_connect(&mathToO, &math.output);
    di_connect(&mathToO, &o.input);

    DiSimulation *simulation = di_simulation_create();

    DiSignal signal1;
    DiSignal signal2;

    di_signal_init(&signal1, 8);
    di_signal_init(&signal2, 8);

    bit_string::copy_bit_string_to_signal(&signal1, "01101111");
    bit_string::copy_bit_string_to_signal(&signal2, "00010001");

    di_input_set(&a, signal1, simulation);
    di_input_set(&b, signal2, simulation);

    di_simulation_run(simulation, 100);

    auto value = bit_string::signal_to_bit_string(&o.signal);

    REQUIRE(value == "01011110");

    di_simulation_free(simulation);

    di_node_destroy(&aToMath);
    di_node_destroy(&bToMath);
    di_node_destroy(&mathToO);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_arithmetic_destroy(&math);
    di_output_destroy(&o);
}

TEST_CASE("Test Arithmetic Mul") {
    DiInput a;
    DiInput b;

    DiArithmetic math;

    DiOutput o;

    DiNode aToMath;
    DiNode bToMath;

    DiNode mathToO;

    di_input_init(&a, 8);
    di_input_init(&b, 8);
    di_arithmetic_init(&math, 8, DI_ARITHMETIC_OP_MUL);
    di_output_init(&o, 8);

    di_node_init(&aToMath, 8);
    di_node_init(&bToMath, 8);
    di_node_init(&mathToO, 8);

    di_connect(&aToMath, &a.output);
    di_connect(&aToMath, &math.in_a);

    di_connect(&bToMath, &b.output);
    di_connect(&bToMath, &math.in_b);

    di_connect(&mathToO, &math.output);
    di_connect(&mathToO, &o.input);

    DiSimulation *simulation = di_simulation_create();

    DiSignal signal1;
    DiSignal signal2;

    di_signal_init(&signal1, 8);
    di_signal_init(&signal2, 8);

    bit_string::copy_bit_string_to_signal(&signal1, "00001000");
    bit_string::copy_bit_string_to_signal(&signal2, "00000011");

    di_input_set(&a, signal1, simulation);
    di_input_set(&b, signal2, simulation);

    di_simulation_run(simulation, 100);

    auto value = bit_string::signal_to_bit_string(&o.signal);

    REQUIRE(value == "00011000");

    di_simulation_free(simulation);

    di_node_destroy(&aToMath);
    di_node_destroy(&bToMath);
    di_node_destroy(&mathToO);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_arithmetic_destroy(&math);
    di_output_destroy(&o);
}

TEST_CASE("Test Arithmetic Mul Signed") {
    DiInput a;
    DiInput b;

    DiArithmetic math;

    DiOutput o;

    DiNode aToMath;
    DiNode bToMath;

    DiNode mathToO;

    di_input_init(&a, 8);
    di_input_init(&b, 8);
    di_arithmetic_init(&math, 8, DI_ARITHMETIC_OP_MUL_SIGNED);
    di_output_init(&o, 8);

    di_node_init(&aToMath, 8);
    di_node_init(&bToMath, 8);
    di_node_init(&mathToO, 8);

    di_connect(&aToMath, &a.output);
    di_connect(&aToMath, &math.in_a);

    di_connect(&bToMath, &b.output);
    di_connect(&bToMath, &math.in_b);

    di_connect(&mathToO, &math.output);
    di_connect(&mathToO, &o.input);

    DiSimulation *simulation = di_simulation_create();

    DiSignal signal1;
    DiSignal signal2;

    di_signal_init(&signal1, 8);
    di_signal_init(&signal2, 8);

    bit_string::copy_bit_string_to_signal(&signal1, "11111110");
    bit_string::copy_bit_string_to_signal(&signal2, "00000010");

    di_input_set(&a, signal1, simulation);
    di_input_set(&b, signal2, simulation);

    di_simulation_run(simulation, 100);

    auto value = bit_string::signal_to_bit_string(&o.signal);

    REQUIRE(value == "11111100");

    di_simulation_free(simulation);

    di_node_destroy(&aToMath);
    di_node_destroy(&bToMath);
    di_node_destroy(&mathToO);

    di_input_destroy(&a);
    di_input_destroy(&b);
    di_arithmetic_destroy(&math);
    di_output_destroy(&o);
}
