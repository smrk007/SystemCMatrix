#ifndef object
#define object

#include <systemc.h>

SC_MODULE (test) {

	sc_in<int> input;
	sc_out<int> output;

	void function();

	SC_CTOR(test) {
		SC_METHOD(function);
	}
};

#endif 
