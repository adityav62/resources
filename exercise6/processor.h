#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <iostream>
#include <regex>
#include <string>
#include <iomanip>
#include <random>

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>


class processor : public sc_module, tlm::tlm_bw_transport_if<>
{
private:
	std::ifstream file;
	sc_time cycleTime;
	tlm_utils::tlm_quantumkeeper quantumKeeper;

	// Method:
    void processTrace();
    void processRandom();

public:
	tlm::tlm_initiator_socket<> iSocket;

	processor(sc_module_name, std::string pathToTrace, sc_time cycleTime);

	SC_HAS_PROCESS(processor);

	// Dummy method:
    void invalidate_direct_mem_ptr(sc_dt::uint64, sc_dt::uint64)
	{
		SC_REPORT_FATAL(this->name(), "invalidate_direct_mem_ptr not implement");
	}

	// Dummy method:
    tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &,tlm::tlm_phase &, sc_time &)
	{
		SC_REPORT_FATAL(this->name(), "nb_transport_bw is not implemented");
		return tlm::TLM_ACCEPTED;
	}
};

processor::processor(sc_module_name, std::string pathToFile, sc_time cycleTime) :
	file(pathToFile), cycleTime(cycleTime)
{
	if (!file.is_open())
		SC_REPORT_FATAL(name(), "Could not open trace");

    SC_THREAD(processRandom);
	quantumKeeper.set_global_quantum(sc_time(100000,SC_NS)); // ???????
    quantumKeeper.reset();//?????????????????????????????????????????

	//'this' points to the current instance of processor class
	//dereferencing it with '*this' gives us the instance of the processor itself - 
	//in essence, this line binds the initiator socket to the module
	iSocket.bind(*this);
}

// Use the command below in a termial to get your gcc version.
// $ gcc --version
// If it is less than 4.9 uncomment the definition that follows.
// #define GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX

void processor::processTrace()
{
	wait(SC_ZERO_TIME);

    std::string line;
	tlm::tlm_generic_payload trans;
	uint64_t cycles = 0;
	uint64_t address = 0;
	std::string dataStr;
	unsigned char data[4];
	bool read = true;

    while (std::getline(file, line)) //reads each line of the file until there are no more lines to read
	{
#ifdef GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX
		// Available GCC is too old and it does not have support to regular
		// expressions (version <= 4.9).
		std::string time;
		std::string command;
		std::string addr;
		std::istringstream iss(line);
		// Get the timestamp for the transaction.
		iss >> time;
		if (time.empty()) {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
		cycles = std::stoull(time.c_str(), nullptr, 10);

		// Get the command.
		iss >> command;
		if (command.empty()) {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
		if (command == "read") {
			read = true;
		} else if (command == "write") {
			read = false;
		} else {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}

		// Get the address.
		iss >> addr;
		if (addr.empty()) {
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
		address = std::stoull(addr.c_str(), nullptr, 16);

		// Get the data if transaction is a write.
		if (read == false) {
			iss >> dataStr;
			if (dataStr.empty()) {
				SC_REPORT_FATAL(name(), "Syntax error in trace");
			}
			unsigned long long d = std::stoull(dataStr.c_str(), nullptr, 16);
			data[0] = (unsigned char)((d & 0xff000000) >> 24);
			data[1] = (unsigned char)((d & 0x00ff0000) >> 16);
			data[2] = (unsigned char)((d & 0x0000ff00) >> 8);
			data[3] = (unsigned char)((d & 0x000000ff) >> 0);
		}

#else
		// Available GCC has support to regular expressions (version >= 4.9)
		std::regex reW("(\\d+)\\s*:\\s*write\\s+0x([\\d\\w]+)\\s+0x([\\d\\w]+)");//RegEx for WRITE
		std::regex reR("(\\d+)\\s*:\\s*read\\s+0x([\\d\\w]+)"); //RegEx for READ
		std::smatch matchW;
		std::smatch matchR;

		if (std::regex_search(line, matchW, reW) && matchW.size() > 1)
		{
			//If a line matches a write operation, it extracts the cycle number, 
			//the address, and the data from the line, and sets read to false
			cycles = std::stoull(matchW.str(1), nullptr, 10);
			read = false;
			address = std::stoull(matchW.str(2), nullptr, 16);
			dataStr = matchW.str(3);
			for(int i = 0; i < 4; i++)
			{
				data[i] = (unsigned char) std::stoi(dataStr.substr(i * 2, 2),
						nullptr,
						16);
			}
		}
		else if (std::regex_search(line, matchR, reR) && matchR.size() > 1)
		{
			//If a line matches a read operation, it extracts the cycle number 
			//and the address from the line, and sets read to true
			cycles = std::stoull(matchR.str(1), nullptr, 10);
			read = true;
			address = std::stoull(matchR.str(2), nullptr, 16);
		}
		else
		{
			//If a line doesn’t match either operation, it reports a fatal error - inlcuding memory out of range
			SC_REPORT_FATAL(name(), "Syntax error in trace");
		}
#endif /* GCC_LESS_THAN_4_9_DOES_NOT_SUPPORT_REGEX */


		sc_time delay;
		//delay of the operation

		//If the current simulation time is less than or equal 
		//to the cycle number times the cycle time, it sets the 
		//delay to the difference between the two. Otherwise, it sets the delay to zero.
		if (sc_time_stamp() <= cycles * cycleTime)
		{
			delay = cycles * cycleTime - sc_time_stamp();
		}
		else
		{
            delay = SC_ZERO_TIME;
		}

		//a TLM generic payload is set up with the address, data length, 
		//command (read or write), and data pointer, and is sent to the 
		//target module using the b_transport method of the initiator socket.
		trans.set_address(address);
		trans.set_data_length(4);
		trans.set_command(read ? tlm::TLM_READ_COMMAND : tlm::TLM_WRITE_COMMAND);
		trans.set_data_ptr(data);
		iSocket->b_transport(trans, delay);

		if(trans.get_response_status() != tlm::TLM_OK_RESPONSE)
		{
			//td::cout << trans.get_response_string() << std::endl;
			//SC_REPORT_WARNING(name(), "See if data ")
			SC_REPORT_FATAL(name(), "Error Occured");
		}
		wait(delay);

		//the following lines are used to just print out some information about the operation
#if 1
		std::cout << std::setfill(' ') << std::setw(4)
			<< name() << " "
			<< std::setfill(' ') << std::setw(10)
			<< sc_time_stamp() << " "
			<< std::setfill(' ') << std::setw(5)
			<< (read ? "read" : "write") << " 0x"
			<< std::setfill('0') << std::setw(8)
			<< address
			<< " 0x" << std::hex
			<< std::setfill('0') << std::setw(2)
			<< (unsigned int)data[0]
			<< std::setfill('0') << std::setw(2)
			<< (unsigned int)data[1]
			<< std::setfill('0') << std::setw(2)
			<< (unsigned int)data[2]
			<< std::setfill('0') << std::setw(2)
			<< (unsigned int)data[3]
			<< std::endl;
#endif
	}

	// End Simulation because there are no events.
}

void processor::processRandom()
{
    wait(SC_ZERO_TIME);

    tlm::tlm_generic_payload trans;

    uint64_t cycles;
    uint64_t address;
    unsigned char data[4];

    std::default_random_engine randGenerator;
    std::uniform_int_distribution<uint64_t> distrCycle(0, 99);
    std::uniform_int_distribution<uint64_t> distrAddr(0, 1023);

	//The next few lines set up a TLM generic payload with a data length of 4, 
	//a write command, and a data pointer to an array of zeros.
    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    
    trans.set_data_length(4);
    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_data_ptr(data);

    for (uint64_t transId = 0; transId < 100000000; transId++) //This line starts a loop that will execute 100,000,000 transactions.
    {
		sc_time delay = quantumKeeper.get_local_time();

		//inside the loop, a random cycle number and a random address are generated
        cycles = distrCycle(randGenerator);
        address = distrAddr(randGenerator);

		//calculate the delay as the cycle number times the cycle time.
        //sc_time delay = cycles * cycleTime;
		

		//address of the payload is set, and sent to the target module 
		//using the b_transport method of the initiator socket, 
		//with a time annotation of 'delay'.
        trans.set_address(address);
        iSocket->b_transport(trans, delay);
		quantumKeeper.set(delay);
		if(quantumKeeper.need_sync())
		{
			quantumKeeper.sync();
			
		}

        wait(delay);
    }
	

    // End Simulation because there are no events.
}

#endif // PROCESSOR_H
