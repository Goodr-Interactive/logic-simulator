#ifndef DIGISIM_EXTENDER_H
#define DIGISIM_EXTENDER_H

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

struct Extender {
    DiInput input;
    DiOutput output;

    DiNode inToExtender;
    DiNode extenderToOut;

    DiBitExtender extender;

    DiSignal *simulate(DiSignal *in);

    Extender(DiBitExtenderPolicy policy, size_t in_bits, size_t out_bits);
    ~Extender();
};

#endif // DIGISIM_EXTENDER_H
