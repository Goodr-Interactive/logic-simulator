#ifndef DIGISIM_SR_LATCH_H
#define DIGISIM_SR_LATCH_H

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct SrLatch {
    DiInput set{};
    DiInput reset{};

    DiOutput value{};

    DiOr first{};
    DiOr second{};

    DiNot firstNegate{};
    DiNot secondNegate{};

    DiNode setToFirst{};
    DiNode resetToSecond{};
    DiNode firstToFirstNegate{};
    DiNode secondToSecondNegate{};
    DiNode firstNegateToSecond{};
    DiNode secondNegateToFirst{};

    void addInputs(DiSimulation *simulation);

    SrLatch();
    ~SrLatch();
};

#endif // DIGISIM_SR_LATCH_H
