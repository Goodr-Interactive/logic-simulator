#ifndef DIGISIM_MULTIPLEXER_H
#define DIGISIM_MULTIPLEXER_H

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct Multiplexer {
    DiInput a{};
    DiInput b{};
    DiInput c{};
    DiAnd and1{};
    DiAnd and2{};
    DiOr sum{};
    DiNot flip{};
    DiOutput f{};

    DiNode aToAnd1{};
    DiNode cToAnd2{};
    DiNode bToGates{};
    DiNode flipToAnd2{};
    DiNode and1ToSum{};
    DiNode and2ToSum{};
    DiNode sumToF{};

    void connect();
    void addInputs(DiSimulation *simulation);

    Multiplexer();
    ~Multiplexer();
};

#endif // DIGISIM_MULTIPLEXER_H
