#ifndef DIGISIM_GATES_H
#define DIGISIM_GATES_H

extern "C" {
#include <digisim/element.h>
#include <digisim/elements/all.h>
#include <digisim/node.h>
#include <digisim/simulation.h>
}

#include <optional>

struct UnaryGate {
    DiInput a {};
    DiOutput f {};

    DiNode aToGate {};
    DiNode gateToF {};

    void setInput(DiSimulation *simulation, DiBit value);

    std::optional<DiBit> simulate(DiBit inputValue);
    std::optional<DiBit> simulate(DiSimulation *simulation, DiBit inputValue);

    UnaryGate(DiTerminal *input, DiTerminal *output);
    ~UnaryGate();
};

struct BinaryGate {
    DiInput a;
    DiInput b;
    DiOutput f;

    DiNode aToGate;
    DiNode bToGate;
    DiNode gateToF;

    void setInputs(DiSimulation *simulation, DiBit aValue, DiBit bValue);

    std::optional<DiBit> simulate(DiBit aValue, DiBit bValue);
    std::optional<DiBit> simulate(DiSimulation *simulation, DiBit aValue, DiBit bValue);

    BinaryGate(DiTerminal *first, DiTerminal *second, DiTerminal *output);
    ~BinaryGate();
};

#endif // DIGISIM_GATES_H
