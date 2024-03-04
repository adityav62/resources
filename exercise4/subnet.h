#ifndef SUBNET_H
#define SUBNET_H

#include <systemc.h>

#include "place.h"
#include "transition.h"

// Subnet:
// TODO
template<unsigned int N=1, unsigned int M=1>
SC_MODULE(subnet)
{
    public:
    place<1,1> ACTIVE;
    transition<1,1,1> ACT;
    transition<1,1,0> RD;
    transition<1,1,0> PRE;
    transition<1,1,0> WR;
    sc_port<placeInterface> IDLE;

    SC_CTOR(subnet): ACTIVE(0), ACT("ACT"), RD("RD"), PRE("PRE"), WR("WR"), IDLE("IDLE")
    {

        ACT.in.bind(IDLE);
        ACT.out.bind(ACTIVE);
        ACT.inhibitors.bind(ACTIVE);

        RD.in.bind(ACTIVE);
        RD.out.bind(ACTIVE);

        PRE.in.bind(ACTIVE);
        PRE.out.bind(IDLE);

        WR.in.bind(ACTIVE);
        WR.out.bind(ACTIVE);
    }
};

#endif // SUBNET_H
