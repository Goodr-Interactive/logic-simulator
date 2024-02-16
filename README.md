# Digisim - Logic Simulator

Digisim is a logic simulator library designed for portability and performance.
The library is written in C 

# Building

To build, you first need to have to download [CMake](https://cmake.org) and have any compatible [C compiler](https://clang.llvm.org) and `make` installed.

First, clone the project. Optionally, if you wish to run tests, download all the git submodules.

```sh
# Clone the project (and download the submodule dependencies)
git clone https://github.com/mariobadr/logic-simulator --recurse-submodules
cd logic-simulator
```

Next, create a folder for your CMake build project. On Unix, you can do this like so:
```
mkdir build
cd build
cmake ..
```

Finally, build the project by invoking make or ninja.

```
cmake --build build --target digisim
```

To run tests, build the test target first:
```
cmake --build build --target digisim-tests
```

And run the `digisim-tests` executable, typically in `build/tests/digisim-tests`.

# Example

If `digisim` is cloned locally, you can add digisim to your CMake project like so:
```cmake
add_library(other-project library.c)

add_subdirectory(path/to/digisim)

target_link_libraries(other-project PUBLIC digisim)
```

Now, you can include headers and start simulating!


```c
// For use with C++, encase this in extern "C":
// extern "C" {
#include <digisim/node.h>
#include <digisim/simulation.h>
#include <digisim/elements/all.h>
// }

#include <assert.h>

int main() {
    DiInput a, b;
    DiOutput f;

    DiNode aGate, bGate, gateF;

    DiAnd gate;

    di_input_init(&a, 1);
    di_input_init(&b, 1);
    di_output_init(&f, 1);

    di_and_init(&gate, 1);

    di_node_init(&aGate);
    di_node_init(&bGate);
    di_node_init(&gateF);

    di_connect(&aGate, &a.output);
    di_connect(&aGate, &gate.input_a);

    di_connect(&bGate, &b.output);
    di_connect(&bGate, &gate.input_b);

    di_connect(&gateF, &f.input);
    di_connect(&gateF, &gate.output);

    DiSimulation simulation;

    di_simulation_init(&simulation);
    
    // Two high inputs.
    di_input_set_bit(&a, 0, DI_BIT_HIGH, &simulation);
    di_input_set_bit(&b, 0, DI_BIT_HIGH, &simulation);

    di_simulation_run(&simulation, 100);
    
    // And gate outputs high!
    assert(di_output_get_bit(&f, 0) == DI_BIT_HIGH);

    di_simulation_destroy(&simulation);

    di_node_destroy(&aGate);
    di_node_destroy(&bGate);
    di_node_destroy(&gateF);

    di_and_destroy(&gate);

    di_input_destroy(&a);
    di_input_destroy(&b);

    di_output_destroy(&f);
    
    return 0;
}
```
