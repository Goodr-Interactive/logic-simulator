/**
 * @file
 * @brief Interface for Exposing Element Inputs/Outputs
 */

#ifndef DIGISIM_TERMINAL_H
#define DIGISIM_TERMINAL_H

#include <digisim/signal.h>

#include <stddef.h>

typedef struct di_element_t DiElement;
typedef struct di_node_t DiNode;
typedef struct di_simulation_t DiSimulation;

/**
 * Connection endpoint for all elements/components.
 *
 * To initialize/destroy, use di_terminal_init/di_terminal_destroy.
 */
typedef struct di_terminal_t {
    /**
     * Element that owns this terminal.
     */
    DiElement *parent;

    /**
     * The number of bits that this terminal will emit.
     */
    size_t bits;

    /**
     * The wire that this terminal is connected to.
     *
     * NULL if this terminal is not connected.
     */
    DiNode *node;

    /**
     * If true, then this terminal is outputting a value to `node` and `signal` is initialized.
     */
    bool holding;

    /**
     * The signal that is being output to `node`.
     *
     * Only readable if `holding` is true.
     */
    DiSignal signal;
} DiTerminal;

/**
 * Initialize a DiTerminal struct.
 *
 * @memberof DiTerminal
 * @param terminal Pointer to initialize
 * @param parent The element that owns this terminal
 * @param bits Number of bits for the terminal
 */
void di_terminal_init(DiTerminal *terminal, DiElement *parent, size_t bits);

/**
 * Destroy a DiTerminal struct.
 *
 * @memberof DiTerminal
 * @param terminal Pointer to destroy
 */
void di_terminal_destroy(DiTerminal *terminal);

/**
 * Writes a value to this terminal (holds the connected wire to a value).
 *
 * @memberof DiTerminal
 * @param terminal The terminal to hold
 * @param signal The value to output
 * @param simulation The simulation that will propagate this change
 */
void di_terminal_write(DiTerminal *terminal, DiSignal *signal, DiSimulation *simulation);

/**
 * Holds the terminal to the value in `terminal->signal`.
 *
 * Callers should first write to `terminal->signal`,
 * and then call this method to propagate an event to the connected wire.
 *
 * A terminal can stop holding a value to a wire using di_terminal_reset.
 *
 * @memberof DiTerminal
 * @param terminal The terminal to send a value
 * @param simulation The simulation to propagate the value change
 */
void di_terminal_send(DiTerminal *terminal, DiSimulation *simulation);

/**
 * Stops a terminal from holding a value from a wire (switches to "read" or "inactive" mode).
 *
 * @memberof DiTerminal
 * @param terminal The terminal to reset
 * @param simulation The simulation that will propagate this change
 */
void di_terminal_reset(DiTerminal *terminal, DiSimulation *simulation);

/**
 * Writes a value of all bit to this terminal.
 * Equivalent to di_terminal_write with di_signal_filled.
 *
 * @memberof DiTerminal
 * @param terminal The terminal to write
 * @param bit The bit to fill the signal with
 * @param simulation The simulation that will propagate this change
 */
void di_terminal_fill(DiTerminal *terminal, DiBit bit, DiSimulation *simulation);

/**
 * Reads a value from the terminal.
 * If the connected wire is being held to a value by another terminal, then a DiSignal will be returned.
 * Otherwise, if the terminal is currently being held to a value, that is returned.
 * Otherwise, the `signal` field is cleared to DI_BIT_UNKNOWN and that is returned.
 *
 * @memberof DiTerminal
 * @param terminal The terminal to read from
 * @return The value of the terminal (or NULL if unconnected)
 */
DiSignal *di_terminal_read(DiTerminal *terminal);

/**
 * Similar to di_terminal_read, except the current held value of the terminal does not affect the value of the read.
 * If the terminal is unconnected, then output will be initialized to DI_BIT_UNKNOWN.
 *
 * @param terminal The terminal to read from.
 * @param output The signal to store the output of the read. Should have the same number of bits as terminal.
 */
void di_terminal_directional_read(DiTerminal *terminal, DiSignal *output);

#endif // DIGISIM_TERMINAL_H
