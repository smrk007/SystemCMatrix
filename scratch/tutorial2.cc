#include <iostream>
#include <stdlib.h>
#include <systemc.h>

SC_MODULE (first_counter) {

	// Input and output
	sc_in_clk	clock;
	sc_in<bool>	reset;
	sc_in<bool>	enable;
	sc_out< sc_uint<4> >	counter_out;

	// Local variables
	sc_uint<4>			count;

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

	sc_signal<bool>		clock;
	sc_signal<bool>		reset;
	sc_signal<bool>		enable;
	sc_signal< sc_uint<4> >		counter_out;
	int i = 0;

	first_counter counter("counter");
		counter.clock(clock);
		counter.reset(reset);
		counter.enable(enable);
		counter.counter_out(counter_out);

	sc_start(1,SC_NS);

	sc_trace_file *wf = sc_create_vcd_trace_file("counter");
	sc_trace(wf, clock, "clock");
	sc_trace(wf, reset, "reset");
	sc_trace(wf, enable, "enable");
	sc_trace(wf, counter_out, "count");

	reset = 0;
	enable = 0;
	for (i = 0; i < 5; i++) {
		clock = 0;
		sc_start(1,SC_NS);
		clock = 1;
		sc_start(1,SC_NS);
	}

	// Reset
	reset = 1;
	std::cout << "@" << sc_time_stamp() << " Asserting reset" << std::endl;
	for (i = 0; i < 10; i++) {
		clock = 0;
		sc_start(1,SC_NS);
		clock = 1;
		sc_start(1,SC_NS);
	}

	// Un-Reset
	reset = 0;
	std::cout << "@" << sc_time_stamp() << " De-Asserting reset" << std::endl;
	for (i = 0; i < 5; i++) {
		clock = 0;
		sc_start(1,SC_NS);
		clock = 1;
		sc_start(1,SC_NS);
	}

	std::cout << "@" << sc_time_stamp() << " Asserting Enable" << std::endl;
	enable = 1;
	for ( i = 0; i < 20; i++) {
		clock = 0;
		sc_start(1,SC_NS);
		clock = 1;
		sc_start(1,SC_NS);
	}

	std::cout << "@" << sc_time_stamp() << " De-Asserting Enable" << std::endl;
	enable = 0;

	std::cout << "@" << sc_time_stamp() << " Terminating simulation" << std::endl;
	sc_close_vcd_trace_file(wf);
	return 0;
}
