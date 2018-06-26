#include <iostream>
#include <systemc.h>

SC_MODULE (first_counter) {

	// Input and output
	sc_in_clk	clock;
	sc_in<bool>	reset;
	sc_in<bool>	enable;
	sc_out<int>	counter_out;

	// Local variables
	int			count;

	// Code
	void increment () {
		if (reset.read() == 1) {
			count = 0;
			counter_out.write(count);
		} else if (enable.read() == 1) {
			count = count + 1;
			counter_out.write(count);
			std::cout << "@" << sc_time_stamp() << " :: Incremented Counter ";
			std::cout << counter_out.read() << std::endl;
		}
	}

	// Constructor
	SC_CTOR (first_counter) {
		std::cout << "Executing new" << std::endl;
		SC_METHOD (increment);
		sensitive << reset;
		sensitive << clock.pos();
	}
	
};

int sc_main(int argc, char* argv[]) {

	

	return 0;
}
