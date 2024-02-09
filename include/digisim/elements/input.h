/**
 * @file
 * @brief Input Terminal Element and Connections
 */

#ifndef DIGISIM_ELEMENTS_INPUT_H
#define DIGISIM_ELEMENTS_INPUT_H

#include <digisim/element.h>
#include <digisim/terminal.h>

/**
 * Input element, for holding a node/wire to a value.
 *
 * To initialize/destroy, use di_input_init/di_input_destroy.
 */
typedef struct di_input_t {
    /**
     * Element descriptor
     */
    DiElement element;

    /**
     * Number of bits for the output terminal
     */
    size_t bits;

    /**
     * Current signal that is emitting through `output`
     *
     * Set this field using di_input_set/di_input_set_bit.
     */
    DiSignal signal;

    /**
     * Output terminal
     *
     * This terminal is held to the value of `signal`.
     */
    DiTerminal output;
} DiInput;

/**
 * Initialize a DiInput struct.
 *
 * @memberof DiInput
 * @param input Pointer to initialize
 * @param bits Bit-size for the output terminal
 */
void di_input_init(DiInput *input, size_t bits);

/**
 * Destroy a DiInput struct.
 *
 * @memberof DiInput
 * @param input Pointer to destroy
 */
void di_input_destroy(DiInput *input);

/**
 * Write the value in `signal` to the `output` terminal.
 *
 * This method should be used to add an input to a simulation that has just been created.
 * Simulations do not propagate changes unless they are added.
 * This method can be used to force a simulation to recognize a change in input.
 *
 * If a di_input_set method was used instead, this method does not need to be called.
 *
 * @memberof DiInput
 * @param input Input element
 * @param simulation Simulation
 */
void di_input_emit(DiInput *input, DiSimulation *simulation);

/**
 * Returns the bit at `index` being emitted by this input.
 *
 * @memberof DiInput
 * @param input Input element
 * @param index Index of the bit. (index < `input->bits`)
 * @return The value of the bit at this index
 */
DiBit di_input_get_bit(DiInput *input, size_t index);

/**
 * Changes one bit of the value of this input.
 *
 * @memberof DiInput
 * @param input Input element
 * @param index Index of the bit to change. (index < `input->bits`)
 * @param bit The new value of the bit
 * @param simulation Simulation that will propagate the change
 */
void di_input_set_bit(DiInput *input, size_t index, DiBit bit, DiSimulation *simulation);

/**
 * Changes the value of this input.
 *
 * @memberof DiInput
 * @param input Input element
 * @param move_signal The new value of this input
 * @param simulation Simulation that will propagate the change
 */
void di_input_set(DiInput *input, DiSignal move_signal, DiSimulation *simulation);

#endif // DIGISIM_ELEMENTS_INPUT_H
