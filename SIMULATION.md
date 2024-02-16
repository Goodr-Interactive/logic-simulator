# Simulation Algorithm and Stepping

Digisim's simulation algorithm is event based.
This means that Digisim waits for an input for a gate to change before recalculating it.
Changes are processed in an order defined by the `DiSimulation` struct.

# Queue - Unit Delay Simulation

This is the primary simulation method used by digisim.
This simulation mode assumes all elements in the circuit take the same amount of time to respond to an impulse.
This unit delay behaviour is done through a circular buffer queue, and propagation bursts called "steps."

The queue stores a list of `DiNode *` or whose values have recently been changed.
When a wire's value has been potentially changed, the `di_node_changed` method is invoked.
This method will recalculate the value on the wire from all the connected terminals, and invoke `di_simulation_add` if the value has changed.
It is important to check if the wire value has actually changed before adding it back to the queue, as otherwise stable but cyclic circuits may never stabilize.

Performing one step of simulation is equivalent to one unit of time.
After a step, each gate that had a pending change will have a chance to react and change its outputs.
This event propagation algorithm is very similar to BFS.

There are some timing quirks with a unit delay circuit.
For example, an SR Latch is perfectly symmetrical, so without a set/reset signal it is astable and will never converge.
These issues can be resolved by adding a "buffer" component to delay one signal further, or by using the zero delay strategy.

One step can be broken down into the following procedure:

1. For each `DiNode *` in the queue at the start of the step, repeat the following items.
   New items added to the queue during this step are saved but ignored.
2. Alert all connected `DiTerminal *` terminals and their elements that the value of the wire has changed.
   This is done through `di_node_propogate`.
3. Elements whose inputs may have changed are alerted through their `DiChangedCallback`.
   They can now re-read the values of their input terminals, and output new values to their output terminals.
4. Output terminals whose values have changed in the update will call `di_node_changed` on the wire.
5. If the overall value of a wire has changed, the `di_node_changed` method will add this wire to the simulation queue so the change can be propagated further through the circuit.

# Stack - Zero Delay Simulation

This is an experimental simulation method used by digisim.
This simulation mode assumes all elements in the circuit propagate a change with zero delay.
This is done through a resizable stack. There is no notion of steps, but there is now instead a maximum exploration depth.

This event propagation algorithm is very similar to DFS.
A zero delay simulation will break timing conflicts (ties) randomly, or by the order of appearance in the netgraph.
The symmetrical SR Latch described in the unit delay section will not be astable, although the exact state will not be predictable.

One simulation run can be broken down into the following procedure:

1. Until the stack is empty, pop a `DiNode *` object. The simulation should now keep track of the current depth of this node.
   If the depth of this node exceeds the max depth parameter of the algorithm, then the algorithm will skip this node, mark the simulation as astable, and continue through the stack.
2. Alert all connected `DiTerminal *` terminals and their elements that the value of the wire has changed.
   Similarly to the unit delay, this is done through `di_node_propogate`.
3. Elements whose inputs may have changed are alerted through their `DiChangedCallback`.
   They can now re-read the values of their input terminals, and output new values to their output terminals.
4. Output terminals whose values have changed in the update will call `di_node_changed` on the wire.
5. If `di_node_changed` deems necessary, it will add itself to the queue. It will add itself with a depth that is one higher than the wire that propagated the change that changed this wire.
