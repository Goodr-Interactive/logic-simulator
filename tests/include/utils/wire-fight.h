#ifndef DIGISIM_WIRE_FIGHT_H
#define DIGISIM_WIRE_FIGHT_H

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct WireFight {
    DiInput fighter_a{};
    DiInput fighter_b{};

    DiOutput value{};

    DiNode connector{};

    explicit WireFight(size_t bits);
    ~WireFight();
};

#endif // DIGISIM_WIRE_FIGHT_H
