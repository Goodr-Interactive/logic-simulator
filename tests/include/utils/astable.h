#ifndef DIGISIM_ASTABLE_H
#define DIGISIM_ASTABLE_H

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct Astable {
    DiInput input{};

    DiNode nodes[3]{};
    DiNot gates[3]{};

    Astable();
    ~Astable();
};

#endif // DIGISIM_ASTABLE_H
