#ifndef PLACE_H
#define PLACE_H

#include <systemc.h>

// Place Interface:
// TODO
//template<class T>
class placeInterface: public sc_interface
{
    public:
    //virtual void addTokens(unsigned int n) = 0;
    //virtual void removeTokens(unsigned int n) = 0;
    //virtual unsigned int testTokens() = 0;
    virtual void addTokens() = 0;
    virtual void removeTokens() = 0;
    virtual bool testTokens() = 0;

};

// Place Channel:
//Win = input weigt of place, Wout = output weight of place
template<unsigned int Win = 1, unsigned int Wout = 1>
class place: public placeInterface
{
    private:
    unsigned int tokens; //current number of tokens in the place

    public:
    place(unsigned int val){
        tokens = val;
    }

    void addTokens(){
        tokens = tokens + Win;
    }

    void removeTokens(){
        tokens = tokens - Win;
    }

    bool testTokens()
    {
        if(tokens >= Wout)
        {
            return true;
        }
        else
            return false;
    }
};

#endif // PLACE_H
