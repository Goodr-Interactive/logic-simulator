#include <digisim/simulation.h>

#include <digisim/node.h>
#include <digisim/simulations/unit-delay.h>
#include <digisim/simulations/zero-delay.h>

#include <stdlib.h>

bool di_simulation_run(DiSimulation *simulation, size_t max_depth) {
    if (simulation) {
        return simulation->run(simulation, max_depth);
    }

    return true; // dead
}

void di_simulation_add(DiSimulation *simulation, DiNode *node) {
    if (simulation) {
        simulation->add(simulation, node);
    }
}

DiSimulation *di_simulation_create() {
    // Default simulation is the unit simulation.
    //    DiUnitSimulation *simulation = malloc(sizeof(DiUnitSimulation));
    //
    //    di_unit_simulation_init(simulation);
    //
    //    return (DiSimulation *)simulation;

    DiZeroSimulation *simulation = malloc(sizeof(DiZeroSimulation));

    di_zero_simulation_init(simulation);

    return (DiSimulation *)simulation;
}

void di_simulation_free(DiSimulation *simulation) {
    //    DiUnitSimulation *unit = (DiUnitSimulation *)simulation;
    //
    //    di_unit_simulation_destroy(unit);
    //
    //    free(unit);

    DiZeroSimulation *zero = (DiZeroSimulation *)simulation;

    di_zero_simulation_destroy(zero);

    free(zero);
}
