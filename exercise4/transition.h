#ifndef TRANSITION_H
#define TRANSITION_H

#include <systemc.h>

#include "place.h"

// Transition:
// TODO
//task2 - Multiports, N = input ports, M = output ports, L = inhibitor ports
template<unsigned int N = 1, unsigned int M = 1, unsigned int L=0>
SC_MODULE(transition){


    public:
    sc_port<placeInterface, N, SC_ALL_BOUND> in;
    sc_port<placeInterface, M, SC_ALL_BOUND> out;
    sc_port<placeInterface, L, SC_ZERO_OR_MORE_BOUND> inhibitors;


    SC_CTOR(transition){};
    
    void fire()
    {
        bool fireFlag = true;
        bool inhib = true; //none of the inhibitor inputs have tokens
        for(unsigned int i = 0; i < N; i++)
        {
            if(in[i]->testTokens() == false) //even if one input port does not have token, it can not fire
            {   
                fireFlag = false;
                break;
            };
        }

        for(unsigned int i = 0; i < L; i++)
        {
            if(inhibitors[i]->testTokens() == true) //even if one input port does not have token, it can not fire
            {   
                inhib = false;
                break;
            };
        }
        //if the fireFlag stays true, then the transition can fire, else it can't
        if(fireFlag && inhib) //input ports have tokens and inhibitors don't
        {
            std::cout << this->name() << " Fired" <<std::endl;
            for(unsigned int j = 0; j < N ; j++)
            {
                in[j]->removeTokens();
            }
            for(unsigned int j = 0; j < M ; j++)
            {
                out[j]->addTokens();
            }
        }
        else
        {
            std::cout << this->name() << " NOT Fired" << std::endl;
        }
        
    }

};

#endif // TRANSITION_H
