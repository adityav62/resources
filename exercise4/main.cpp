#include <systemc.h>

#include "place.h"
#include "transition.h"
#include "subnet.h"

// Toplevel:
// TODO

SC_MODULE(toplevel)
{
    public:
    place<1,1> IDLE;
    subnet<1,1> s1,s2;
    

    SC_CTOR(toplevel): IDLE(2),s1("s1"), s2("s2")//, ACTIVE(0), ACT("ACT"), RD("RD"), PRE("PRE"), WR("WR")
    {   
        s1.IDLE.bind(IDLE);
        s2.IDLE.bind(IDLE);

        SC_THREAD(process);
    }

    void process()
    {
        while(true)
        {
            wait(10, SC_NS);
            s1.ACT.fire();
            wait(10, SC_NS);
            s1.ACT.fire();
            wait(10, SC_NS);
            s1.RD.fire();
            wait(10, SC_NS);
            s1.WR.fire();
            wait(10, SC_NS);
            s1.PRE.fire();
            wait(10, SC_NS);
            s1.ACT.fire();
            wait(10, SC_NS);
            s2.ACT.fire();
            wait(10, SC_NS);
            s2.ACT.fire();
            wait(10, SC_NS);
            s1.PRE.fire();
            wait(10, SC_NS);
            s2.PRE.fire();
            wait(10, SC_NS);
            sc_stop();
        }
    }

};

int sc_main(int, char**)
{
    // TODO
    toplevel t("t");
    sc_start();
    return 0;
}
