#ifndef KPN_H
#define KPN_H

#include <systemc.h>

SC_MODULE(kpn)
{
private:
    // ADD THINGS HERE
sc_fifo<unsigned int> a,b,c,d;
sc_signal<unsigned int> e;

public:
    SC_CTOR(kpn): a(10), b(10), c(10), d(10) // : ADD THINGS HERE - initialize FIFOs b and c here
    {
        // ADD THINGS HERE
        b.write(1);
        c.write(0);
        SC_THREAD(Split);
        SC_THREAD(Add);
        SC_THREAD(Delay);
        sensitive << a.data_read_event() << a.data_written_event()
                << b.data_read_event() << b.data_written_event()
                << c.data_read_event() << c.data_written_event()
                << d.data_read_event() << d.data_written_event();
    }

    void Delay();
    void Split();
    void Add();
};

#endif // KPN_H
