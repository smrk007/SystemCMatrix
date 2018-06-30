#ifndef _MMULT_TEMP_H
#define _MMULT_TEMP_H

#include <systemc.h>

template <int M1, int S, int M2>
SC_MODULE ( mmult_temp ) {
	// NOTE:
	// M1 is input1 left dimension
	// S is shared dimension
	// M2 is input2 right dimension
	
	// Ports
	sc_in<int> 		input1[M1][S];	
	sc_in<int>		input2[S][M2];
	sc_out<int>		output[M1][M2];

	// Local Variables
	int				temp[M1][M2];

	// Processes
	void clear_temp();
	void run();

	// Constructor
	SC_CTOR (mmult_temp) {
		SC_METHOD (clear_temp);
			dont_initialize();
		SC_METHOD (run);
			dont_initialize();
			// Is sensitive to input 1
			for (int row = 0; row < M1; row++) {
			for (int col = 0; col < S; col++) {
				sensitive << input1[row][col];
			}}
			// Is sensitive to input 2
			for (int row = 0; row < S; row++) {
			for (int col = 0; col < M2; col++) {
				sensitive << input2[row][col];
			}}
	}
};

#endif /* mmult_temp.h */
