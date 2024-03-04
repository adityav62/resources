#ifndef STIMULUS_H
#define STIMULUS_H

#include <systemc.h>

SC_MODULE(stim)
{
public:
    sc_out<bool> A, B;
    sc_in<bool> clk; //task4

    SC_CTOR(stim)
    {
        SC_THREAD(StimGen);
        sensitive << clk.pos(); //task4
    }

private:
    void StimGen()
    {
        wait();//wait(SC_ZERO_TIME);
        A.write(false);
        B.write(false);
        wait();//wait(10, SC_NS);
        A.write(false);
        B.write(true);
        wait();//wait(15, SC_NS);
        A.write(true);
        B.write(false);
        wait();//wait(10, SC_NS);
        A.write(true);
        B.write(true);
        wait();//wait(10, SC_NS);
        A.write(false);
        B.write(false);
        wait();//wait(10, SC_NS);
        sc_stop();
    }
};

#endif
