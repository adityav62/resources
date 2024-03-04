#ifndef NAND_H
#define NAND_H

#include <systemc.h>


// TODO: Insert your code here
SC_MODULE(nand)
{
    sc_in<bool> A;
    sc_in<bool> B;
    sc_out<bool> Z;

    SC_CTOR(nand) : A("A"), B("B"), Z("Z")
    {
        SC_METHOD(do_nand);
        sensitive << A << B;
    }

    void do_nand()
    {
        Z.write(!(A.read() && B.read())); // NAND Gate

    }
};
#endif
