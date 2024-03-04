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


TOPICS DISCUSSED:
RELEVANT EXAMPLES:

--------------------------------------------///TLM Basics///--------------------------------------------------------------------


TOPICS DISCUSSED:
RELEVANT EXAMPLES:
--------------------------------------------///TLM Advanced///------------------------------------------------------------------


TOPICS DISCUSSED:
RELEVANT EXAMPLES:
--------------------------------------------------------------------------------------------------------------------------------
