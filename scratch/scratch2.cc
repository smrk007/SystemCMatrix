#include <iostream>
#include <systemc.h>

SC_MODULE (add) {
	
	// Ports
	sc_in<int>		in1, in2;
	sc_out<int>		out;

	// Methods
	void run() {
		out.write(in1.read() + in2.read());
	}

	SC_CTOR (add) {
		SC_METHOD (run);
			dont_initialize();
			sensitive << in1 << in2;
	}
};

SC_MODULE (add_aggregator) {

	// Ports
	sc_in<int>		in1, in2;
	sc_out<int>		out;

	// Signals
	sc_signal<int>	add1_out;

	// Sub-Modules
	add				add1, add2;

	SC_CTOR (add_aggregator) : add1("add1"), add2("add2") {
		// add1 module
		add1.in1(in1);
		add1.in2(in2);
		add1.out(add1_out);
		// add2 module
		add2.in1(add1_out);
		add2.in2(in2);
		add2.out(out);
	}
};

int sc_main(int argc, char* argv[]) {

	// Signals
	sc_signal<int>		in1, in2, out;

	// Sub-Modules
	add_aggregator adder("ADDER");
		adder.in1(in1);
		adder.in2(in2);
		adder.out(out);
	sc_trace_file *trace_file = sc_create_vcd_trace_file("ADDER_TEST");
		sc_trace(trace_file, in1, "in1");
		sc_trace(trace_file, in2, "in2");
		sc_trace(trace_file, out, "out");
	sc_start(1, SC_NS);

	// Connecting Data
	in1 = 2;
	in2 = 3;
	
	sc_start(1, SC_NS);
	std::cout << out << std::endl;
	
	return 0;
}
