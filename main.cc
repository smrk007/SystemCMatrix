#include <iostream>
#include "matrix2x2.cc"
#include <systemc.h>

int sc_main(int argc, char* argv[]) {

	// Declaring the signals
	sc_signal<int> a, b, c, d;

	// Connecting signals to the matrix
	matrix2x2 test("test");
		test.i1(a);
		test.i2(b);
		test.i3(c);
		test.i4(d);

	sc_trace_file *wf = sc_create_vcd_trace_file("matrix");
	sc_trace(wf, a, "a");
	sc_trace(wf, b, "b");
	sc_trace(wf, c, "c");
	sc_trace(wf, d, "d");

	a = 1;
	b = 2;
	c = 3;
	d = 4;

	test.initialize();

	// Initializing the matrix

	// Testing the functionalities

	return 0;
}
