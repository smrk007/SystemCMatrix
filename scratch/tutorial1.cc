#include <systemc.h>
#include <iostream>

SC_MODULE (hello_world) {
	SC_CTOR (hello_world) {
		// System C Constructor
	}

	void say_hello () {
		std::cout << "Hello World!" << std::endl;
	}
};

int sc_main(int argc, char* argv[]) {
	
	hello_world hello("hello");
	hello.say_hello();

	return 0;
}
