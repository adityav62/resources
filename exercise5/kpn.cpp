#include <unistd.h>
#include <systemc.h>

#include "kpn.h"

// ADD THINGS HERE
void kpn::Split()
{
    int counter = 0;
    while(counter < 10)
    {   //FIFOs have indirect wait()
        
        unsigned int temp;
        temp = b.read();
        a.write(temp);
        d.write(temp);
        e.write(temp);
        std::cout << "The output at port e @t = " << sc_time_stamp().to_default_time_units() << " + "<<  sc_delta_count_at_current_time() << " is " << temp <<std::endl;
        counter++;
    }
    sc_stop();
}

void kpn::Add()
{
    while(true)
    {   //FIFOs have indirect wait()
        b.write(a.read() + c.read());
    }
}

void kpn::Delay()
{
    while(true)
    {   //FIFOs have indirect wait()
        c.write(d.read());
    }
}



        