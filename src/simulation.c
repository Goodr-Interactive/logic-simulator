#include <digisim/simulation.h>

#include <digisim/node.h>
#include <digisim/simulations/zero-delay.h>

#include <stdlib.h>

bool di_simulation_run(DiSimulation *simulation, size_t max_depth) {
    return simulation->run(simulation, max_depth);
}

void di_simulation_clear(DiSimulation *simulation) {
    simulation->clear(simulation);
}

void di_simulation_add(DiSimulation *simulation, DiNode *node) {
    if (simulation) {
        simulation->add(simulation, node);
    }
}

DiSimulation *di_simulation_create() {
    DiZeroSimulation *simulation = malloc(sizeof(DiZeroSimulation));

    di_zero_simulation_init(simulation);

    return (DiSimulation *)simulation;
}

void di_simulation_free(DiSimulation *simulation) {
    DiZeroSimulation *zero = (DiZeroSimulation *)simulation;

    di_zero_simulation_destroy(zero);

    free(zero);
}
