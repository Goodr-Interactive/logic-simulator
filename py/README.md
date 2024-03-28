# Digisim Python CLI

Digisim has a Python CLI for parsing and simulating existing Logisim files.
When passed a Logisim `.circ` file, the CLI will parse it, assemble a Digisim netgraph, and then provide an interface for simulating or generating waveforms from the circuit.

Digisim currently supports two subcommands:
 - `table` which generates a truth table for a combinational circuit.
 - `wave` which generates a waveform from a sequence of inputs defined in a waveform JSON file.

You can invoke the Digisim CLI by running the `main.py` file in this folder.

```shell
$ python3 main.py -h

usage: digisim [-h] {table,wave} ...

Digital Logic Simulation CLI

positional arguments:
  {table,wave}
    table       Generate Truth Table
    wave        Generate Waveform

options:
  -h, --help    show this help message and exit
```

# Generating Truth Tables

The `table` subcommand allows you to generate truth tables with the Digisim CLI.
To invoke it, simply pass a path to an existing Logisim file:

```shell
$ python3 main.py table path/to/logisim.circ

{"pins": [{"name": "input_0", "kind": "input"}, {"name": "input_1", "kind": "input"}, {"name": "output_2", "kind": "output"}], "values": [{"input_0": "0", "input_1": "0", "output_2": "0"}, {"input_0": "0", "input_1": "1", "output_2": "1"}, {"input_0": "1", "input_1": "0", "output_2": "1"}, {"input_0": "1", "input_1": "1", "output_2": "0"}]}
```

The `table` command will automatically detect the main circuit in this file, it's inputs and outputs, and generate a JSON.
You can find a schema for the JSON output of this command [here](schemas/truth-table-out.schema.json).

The following options are available for the `table` command:

 - `--circuit <circuit_name>`: Generate the truth table for the `<circuit_name>` within the Logisim project file. By default, the main circuit specified in the Logisim file is used.
 - `--max-depth`: Defines the maximum number of wires deep a change should be propagated before giving up (determining this circuit is ASTABLE). The unit is nodes (how many wires a signal can go through).
 - `--readable`: Generates a human-readable output instead of JSON (ASCII table).

# Generating Waveforms

The `wave` command allows you to analyze how circuits behave over time with the Digisim CLI.
This command is meant for use with sequential circuits.
To invoke it, pass a path to an existing Logisim file **and** a waveform file with the `--waveform` option.

```sh
$ python3 wave path/to/logisim.circ --waveform path/to/wave.json

{"steps": [{"MYREG": "00000000", "VALUE": "00000000"}, {"MYREG": "00000110", "VALUE": "00000110"}, {"MYREG": "11111110", "VALUE": "11111110"}, {"MYREG": "00110000", "VALUE": "00110000"}]}
```

A waveform file has 3 properties:

 - `waveform`: An array of operations to perform in a sequence. Each item is an object where the key is an input pin or data and the value is either. The value of the requested output pins are reported after each item in the sequence.
   - A bit string to set the input pin to ex. `"1010010"` (MSB first)
   - The string `"request"`, which will mark a specific output pin/registers to be queried and reported after the input pins are set and simulation is performed.
 - `request`: An optional array of pins/registers that should always be queried and reported after every step.
 - `clock`: An optional name of a pin that should be toggled OFF and ON each step before reporting the results of the requested output pins.

You can find the full schema for the waveform input file [here](schemas/waveform-in.schema.json).

Example `wave.json`:
```json
{
	"waveform": [
		{ "DATA": "0000000" },
		{ "DATA": "0000011" },
		{ "DATA": "1111111" },
		{ "DATA": "0011000" }
	],
	"request": ["MYREG", "VALUE"],
	"clock": "CLOCK"
}
```

The output reported by the `wave` command is described [here](schemas/waveform-out.schema.json).

The following options are available for the `wave` command (currently the same as the `table` command):

 - `--circuit <circuit_name>`: Generate the truth table for the `<circuit_name>` within the Logisim project file. By default, the main circuit specified in the Logisim file is used.
 - `--max-depth`: Defines the maximum number of wires deep a change should be propagated before giving up (determining this circuit is ASTABLE). The unit is nodes (how many wires a signal can go through).
 - `--readable`: Generates a human-readable output instead of JSON (ASCII table).
