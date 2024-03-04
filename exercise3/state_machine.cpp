#include <iostream>

#include "state_machine.h"
void stateMachine::process()
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
                cnt_where++;
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
                cnt_where++;
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
                cnt_where++;
                break; 

            case GAA:
                if(input.read() == 'G'){
                    state = GAAG;
                }
                else if (input.read() == 'C' || 'T') //(input.read() == 'A' || 'C' || 'T')
                {
                    state = Start;
                }
                else if (input.read() == 'A')
                {
                    cnt_where = cnt_where + 0; //keep looping in this state as long as A persists
                }
                cnt_where++;
                break; 

            case GAAG:
                cnt_often++;
                std::cout<< "Sequence " << cnt_often << "found @"<< sc_time_stamp() << " at position number: " << cnt_where - 3 <<std::endl;
                if(input.read() == 'G'){
                    state = G;
                }
                else if (input.read() == 'A' || 'C' || 'T')
                {
                    state = Start;
                }
                cnt_where++;
                break;

            default:
                state = Start;
         
        }
    }
// TODO