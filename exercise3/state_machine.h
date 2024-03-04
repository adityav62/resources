#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <systemc.h>

// TODO
SC_MODULE(stateMachine)
{
    public:
    sc_in<char> input;
    sc_in<bool> clk;
    enum base {Start, G, GA, GAA, GAAG};
    base state;
    int cnt_often, cnt_where;

    SC_CTOR(stateMachine): input("input"), clk("clk")
    {
        state = Start;
        cnt_often = 0;
        cnt_where = 0;
        SC_METHOD(process);
        sensitive << clk.pos();
        dont_initialize();
    }
    
    void process();
    /*
    {
        //If I understand correctly, I need to start from base = Start and change states according to the input
        switch (state)
        {
            case Start:
                if (input.read() == 'G')
                {
                    state = G;
                }
                else if (input.read() == 'A' || 'C' || 'T')
                {
                    state = Start;
                }
                break;
            
            case G:
                if(input.read() == 'A'){
                    state = GA;
                }
                else if (input.read() == 'G')
                {
                    state = G;
                }
                else if (input.read() == 'C' || 'T')
                {
                    state = Start;
                }
                break; 

            case GA:
                if(input.read() == 'A' ){
                    state = GAA;
                }
                else if (input.read() == 'G')
                {
                    state = G;
                }
                else if (input.read() == 'C' || 'T')
                {
                    state = Start;
                }
                break; 

            case GAA:
                if(input.read() == 'G'){
                    state = GAAG;
                }
                else if (input.read() == 'A' || 'C' || 'T')
                {
                    state = Start;
                }
                break; 

            case GAAG:
                std::cout<< "Sequence found @"<< sc_time_stamp() << " in substring number:" <<std::endl;
                if(input.read() == 'G'){
                    state = G;
                }
                else if (input.read() == 'A' || 'C' || 'T')
                {
                    state = Start;
                }
                break;

            default:
                state = Start;
         
        } 
    }
*/
};

#endif // STATE_MACHINE_H


