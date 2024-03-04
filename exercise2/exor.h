#ifndef EXOR_H
#define EXOR_H

#include <systemc.h>

#include "nand.h"

// TODO: Insert your code here

SC_MODULE(exor)
{
    sc_in<bool> A;
    sc_in<bool> B;
    sc_out<bool> Z;

    nand nand1, nand2, nand3, nand4;
    sc_signal<bool> h1, h2, h3;

    SC_CTOR(exor):
                nand1("nand1"), nand2("nand2"), nand3("nand3"), nand4("nand4"),
                A("A"), B("B"), Z("Z"),
                h1("h1"), h2("h2"), h3("h3")
    {
        nand1.A.bind(A);
        nand1.B.bind(B);
        nand1.Z.bind(h1);

        nand2.A.bind(B);
        nand2.B.bind(h1);
        nand2.Z.bind(h2);

        nand3.A.bind(A);
        nand3.B.bind(h1);
        nand3.Z.bind(h3);

        nand4.A.bind(h2);
        nand4.B.bind(h3);
        nand4.Z.bind(Z);

    }
};

#endif // EXOR_H
