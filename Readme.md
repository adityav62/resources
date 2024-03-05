---------------------------------------------///SystemC Basics///-------------------------------------------------------------

TOPICS DISCUSSED: SC_MODULE, SC_CTOR, delta delay (enables the simulation of concurrency in a sequential simulator), stages of SystemC Simulation Kernel, SC_METHOD, SC_THREAD, sc_event, sc_time, dynamic sensitivity

 --For combinational logic or processes that do not need to maintain state, SC_METHOD is typically used. For sequential logic or processes that need to wait for events, SC_THREAD is typically used
 --No wait() statements with SC_METHOD - will crash simulation
 --In SystemC, SC_THREAD processes do not require a sensitivity list because they are designed to be self-scheduling. This means that they control their own execution using wait() statements.
 --SC_METHOD processes are event-driven and use a sensitivity list to specify which events will trigger them.
 
 --Events are caused or fired through the event class member function notify(): - should ideally be done with a time argument 
 --in case of multiple notifications, only the first one is noted, rest are ignored
 --to trigger events more than once, sc_event can't be used, sc_event_queue has to be used

 --wait() statements in SC_THREAD are examples of dynamic sensitivity

-- WE CAN’T BIND PORTS TO PORTS DIRECTLY!! this will lead to problems that we will see in the next lecture - signals are required to interact with SystemC Kernel - at the edges, bindings are allowed without a signal as well because these are special ports that themselves act as signals


RELEVANT EXAMPLES: delta_delay, feedback_loop, swapping_example, thread_example, sc_event_and_queue, clock_generator, not_chain

--------------------------------------------///SystemC Advanced///--------------------------------------------------------------


TOPICS DISCUSSED: data types, and_reduce() methods, Channels, Interfaces, Primitive Channel  - {FIFO, Buffer, Signal}, Dynamic Processes, Report Handling, Callbacks

--signal is special case of Channel - Channels are containers for communication protocols
--Interface defines channel behaviour, Channel implements one or more interfaces
--Specialized Ports such as sc_in, sc_out, sc_inout are used for sc_signal, for RTL modelling and easy use
--sc_signal is a special type of channel - so are sc_fifo and sc_buffer, both being special cases of sc_signal which implement notifications, along with read and write

--signals can be exchanged with a FIFO or a Buffer - because the interface of all these signals and FIFOs is the same, we can interchange the communication models - an example of this is to put in a Ethernet Network or CAN bus or SPI Bus or I2C - modules do not know which channel is there because they use the same interface for write and read
-- sc_in<int> foo; is short MACRO for sc_port< sc_signal_in_if<int> > Foo;
--sc_out<bool> bar; which is short MACRO for sc_port< sc_signal_inout_if<bool> > Bar;
--sc_port has the parameters - Interface (required), N (maximum number of channels to be bound - optional), Policy (optional)

--complex channels like FIFO can only be modeled using SC_THREADS because SC_METHODS can't be used for blocking processes - blocking functions call an internal wait(), which SC_METHOD can't allow (will result in runtime error)
--primitive channels in SystemC = sc_signal, sc_buffer, sc_fifo, sc_mutex, sc_semaphore
--making your own custom primitve channel --> see slide 33 or example 'custom_signal'

--Hierachical Channels (non-primitive, active) --> have logic + usual channel stuff (useful in buses like CAN which use Arbitration)

--sc_buffer:==> The difference to sc_signal is that with sc_buffer an event is generated each time the write() method is called.With sc_signal, an event is only generated if the old and the new value of the signal are different

--! Two Ports cannot be bound to the same channel - for signals and FIFOs in example ‘portarrays’ - doing so gives an error - sc_fifo<T> cannot have more than one writer
--In SystemC, the order of execution of processes within the same delta cycle is not deterministic.
--If ports are created dynamically, use port.size() within module's process function to work on inidividual ports iteratively

--4 kinds of callbacks for sc_module, sc_prim_channel, sc_port and sc_export - see slide 53
◼ before_end_of_elaboration()
◼ end_of_elaboration()
◼ start_of_simulation()
◼ end_of_simulation()

RELEVANT EXAMPLES: custom_fifo, fifo_example, kpn_example, mutex_example, custom_signal, portarrays, multiports, sc_event_and_queue, event_finder, dynamic_processes, reporting, callbacks

--------------------------------------------///TLM Basics///--------------------------------------------------------------------
In TLM, we are not simulating clocks and pins, just the function calls between components - by transfer of packets - from the event perspective, 
the only thing that matters is we know when communication starts and when does it end.

TOPICS DISCUSSED: speed v/s accuracy tradeoff, LT vs AT difference, LT, Generic Payload, Sockets, Time Annotation, Interconnect Component, Temporal Decoupling, Quantum Keeper, DMI, Debug Transport

--Everything in TLM domain is memory mapped
--no need to deal with delta cycling
--In TLM, we will use Call by Reference using a pointer - in TLM, we don’t transfer the entire packet, we just transfer a pointer (a 64 bit number) - makes simulation really fast
--LT faster than AT {as fast as possible, fast enough to build SW, SW dev centric}
--LT implements blocking transport calls
--LT can't model OOO execution and Backpressure
--can't be used to model out-of-order execution
--Processes can run ahead of simulation time (temporal decoupling)

--In nonstandard TLM transaction between producer consumer without interconnect, the consumer does not need a port because it directly implements the transport call defined in transactionInterface.
--Binding Initiator and Targets ==> ports of both derived from sc_port so we bind the iSocket to tSocket
--The key idea of timing annotation is that the recipient is obliged to behave as if it had received the transaction at time sc_time_stamp() + delay
--in LT style, we want to avoid wait() statements as much as possible - instead, try to annotate time on the local variables which are passed with the transport calls and the target can add its own custom delay to it
--If we have multiple transport calls, make the local delay variable keep accumulating the value of delay{pass it as argument to every transport call}, 
and then wait with the accumulated delay value wait(delay) once at the end of a sequence of transports
--In LT, only Initiator calls wait() statements

--Interconnect acts as a Target on Initiator (Producer) side and a Initiator on the Target (Consumer) side
--If there is one initiator and 2 Targets, the Interconnect internally will have two iSockets and one tSocket (because of above)
--needs to be derived from both forward and backward transport interfaces
--Pipelined Transactions or multiple initiator calls concurrently cannot be modelled with LT as transport calls are atomic, so there can be no collisions

--Temporal Decoupling trades accuracy for speed, by reducing number of context switches - process keeps control until end of Quantum - lessens context switches in case of multiple initiators
--Synchronization in Quantum Keeper ==> if there is time left in the quantum, it is consumed by internally calling wait() so that quantum notion is not broken.
--In case of overrun, starts the next transaction with an offset equal to the delay encountered
--smaller the quantum, higher the accuracy, larger the quantum, higher the speed

--DMI used to bypass a detailed Interconnect (probably defined in RTL) - further increase in simulation speed
--Target can set a DMI hint, but before Initiator can know about it, atleast one transaction has to be performed
--Target can even invalidate DMI regions, meaning it can specify certain regions of memory where DMI must not be used.

--Debug Transport used to bypass bootloading process - putting bootloader into memory should not be simulated - so start simulation with everything loaded, thus no delay
--when we use Debug Transport, it does not simulate time, so bootloading process can be done in it, and it won't be accounted towards simulation time.
--so it is similar to b_transport except, it is delay free, has no waits and no event notifications

--to divide memory into multiple banks, use something of the form:
// Calculate the memory size per module
const int memorySizePerModule = totalMemorySize / numModules;

// Determine the module index based on the address
int moduleIndex = trans.get_address() / memorySizePerModule;

// Correct the address for the specific module
trans.set_address(trans.get_address() % memorySizePerModule);

// Call the appropriate socket based on the module index
iSocket[moduleIndex]->b_transport(trans, delay);


RELEVANT EXAMPLES: custom_tlm, tlm_lt_initiator_target, tlm_lt_initiator_interconnect_target, tlm_quantum_keeper, tlm_lt_dmi

--------------------------------------------///TLM Advanced///------------------------------------------------------------------


TOPICS DISCUSSED: AT, 4-cycle non-blocking transport, Backpressure, PEQs, Exclusion Rules, pipelining transactions, Generic Payload Pools or Memory Manager, Early Completion (Blocking Transport with AT - useful for modeling simple I/O where no acknowledgement is required), Simple Sockets, TAgged Simple Sockets, multipassthrough sockets, Modeling Backpressure with Buffers, Payload Extensions

--with AT, we model for accuracy, not speed
--AT uses non-blocking transport ⇒ we make a request and return. The target will notify the initiator when it is ready - 
it is a classical 4-cycle handshake - allows modeling of out-of-order (OOO) cores and backpressures

--backpressure refers to a situation where the producer is generating data faster than the consumer can handle.
--nonblocking transport also sends current phase along with payload object and delay variable
--4 phases of NB communication: BEGIN_REQ, END_REQ, BEGIN_RESP, END_RESP
--nb_transport_fw() handles BEGIN_REQ and END_RESP phases
--nb_transport_bw() handles END_REQ and BEGIN_RESP phases

--Latency can also be introduced between phases by adding the delays - Request Accept Delay, Latency of Target and Response Accept Delay - Latency of Target is the time it takes for Target to fetch what the Initiator needs (have it ready basically) - these delays can be modeled by using the times in the FW and BW transport calls - recall Time Annotation (making a component pretend as if something takes longer than it actually does in order to model real-HW behaviour) - this can be done on either side, not just accumulation and calling of wait(delay) statement on the Initiator side - in that sense, we have more flexibility than Blocking Transport in LT, and we can even have wait() statements on the Target side - to have a much finer granularity for synchronization.

--It is important for the 4 phases be in the order and must occur in the increasing simulation time order + Phases must change with each call
--nb_transport_fw() must not call nb_transport_bw() directly and vice versa - there must be some kind of Synchronization which must be done by ***Payload Event Queues (PEQs) *** 
they just call wait() statements internally
--Target should handle mixed `b_transport` / `nb_transport` to be fully standard compliant

--MM --> new payload object only created when current number not enough - initially, some will br created 
-- Modules that send (or receive) this payload object must increase a reference count (acquire), which signalizes the memory manager that this object is still in use.
--If a module is finished with the payload object, the reference count is decreased (release)
--If the reference count is 0 the payload is freed and goes back into the pool - must be cleaned up before putting back in pool
--b_transport of LT MAY use a Memory Manager
--nb_transport() of AT MUST use a Memory Manager
--Memory Manager is not part of SystemC standard!!! - standard only provides an Interface

--AT models use the PEQs to synchronize incoming calls with the simulation time (instead of calling wait() statements), unlike LT where we add up delays on a local time variable and do temporal decoupling
--PEQs are used at several places - each Initiator and each Target will have its own PEQ in the AT model for timing annotation

--modeling AT handshake similar to blockign transport --> Early Completion shortcut - Initiator sends a BEGIN_REQ, target says, okay done, and immediately responds with TLM_COMPLETED
--Initiator must immediately check the response status of the Generic Payload Object. With TLM_COMPLETED, the caller should always ignore the phase argument as it becomes a Don’t Care (d.c.)

--Simple sockets -->less code, declaring the socket no longer requires inheriting from Target/Initiator Interfaces, just template it with the class name 
--include the tlm_utils\simple_initiator_socket.h
--Instead of implementing dummy functions and callbacks, just mention in the constructor, that we want to register a nonblocking transport BW function with the defined socket, and this function should be equal to &Initiator::nb_transport_bw - this is then implemented in our class and so when NB transport BW is called, the request is forwarded to our implementation of it, via the function pointer
--Advantage? dummy code is avoided, both blocking and nonblocking transport functions can be registered

--Tagged Simple Sockets --> Allows registering the same callback method with several sockets, origin of incoming transactions is distinguished using socket ID
--Interconnect is usually template class for number of target and initiator targets, can be used to model a BUS
--In order to implement a BUS, either use simple target sockets (meaning number of I's and T's are part of template for BUS interconnect) tagged with routing tables - to store relation between input and output ports
--this relation can also be attached to the implementation using an extension - maps can also be used

--Multipassthrough Sockets --> similar to tagged sockets but there is only one socket - also makes use of IDs
-- allows dynamic binding - number of components in system does not have to be known at compile time
-- order of binding determines IDs in the function calls

--Modeling Backpressure with Buffer:
- When modeling a larger buffer, instead of using wait() statements or events for notifications, we can just declare our custom phases as long as they’re only used internally
- `numberOfTransactions` tells us how full the buffer is

RELEVANT EXAMPLES: tlm_memory_manager, tlm_simple_sockets, tlm_multipassthrough_sockets, tlm_at_backpressure, tlm_payload_extensions

--------------------------------------------------------------------------------------------------------------------------------
Petri Nets --> A special kind of arc, the inhibitor arc, is used to reverse the logic of an input place. With an inhibitor arc, the absence of a token in the input place enables, not the presence

