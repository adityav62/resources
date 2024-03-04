#ifndef STIMULI_H
#define STIMULI_H

#include <string>
#include <systemc.h>

SC_MODULE(stimuli) //short for class stimuli: public sc_module
{
public:
    sc_in<bool> clk;
    sc_out<char> output;
    SC_HAS_PROCESS(stimuli);   
    stimuli(const sc_module_name &name);
    int getCounter(){ return counter;} //public getter method for private variable 'counter'

private:
    std::string sequence;
    void process();
    unsigned int counter;
};

#endif // STIMULI_H
