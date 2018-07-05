#ifndef _SYSTEMCMATRIX_H_
#define _SYSTEMCMATRIX_H_

#include <cmath>
#include <systemc.h>

SC_MODULE (matrix_adder) {
	// Matrix-Matrix Logic Unit

	// Ports
	sc_in<int>		input1[2][2];
	sc_in<int>		input2[2][2];
	sc_out<int>		output[2][2];

	// Operation
	void add();

	SC_CTOR (matrix_adder) {
		SC_METHOD (add);
		dont_initialize();
		for (int row = 0; row < 2; row++) {
			for (int col = 0; col < 2; col++) {
				sensitive << input1[row][col] << input2[row][col];
			}
		}
	}
};

template <int M1, int S, int M2>
SC_MODULE (matrix_multiplier) {

	// Ports
	sc_in<int>		input1[M1][S];
	sc_in<int>		input2[S][M2];
	sc_out<int>		output[M1][M2];

	// Variables
	int				temp[M1][M2];

	// Operation
	void multiply() {
		clear_temp();
		// Filling Temp
		for (int i = 0; i < M1; i++) {
		for (int j = 0; j < S; j++) {
		for (int k = 0; k < M2; k++) {
			temp[i][k] += input1[i][j].read() * input2[j][k].read();
		}}}
		// Sending Temp to Output
		for (int row = 0; row < M1; row++) {
			for (int col = 0; col < M2; col++) {
				output[row][col].write(temp[row][col]);
			}
		}
	}

	void clear_temp() {
		for (int row = 0; row < M1; row++) {
			for (int col = 0; col < M2; col++) {
				temp[row][col] = 0;
			}
		}
	}

	SC_CTOR (matrix_multiplier) {
		SC_METHOD (multiply);
		dont_initialize();
		for (int row = 0; row < M1; row++) {
			for (int col = 0; col < S; col++) {
				sensitive << input1[row][col];
			}
		}
		for (int row = 0; row < S; row++) {
			for (int col = 0; col < M2; col++) {
				sensitive << input2[row][col];
			}
		}
		SC_METHOD (clear_temp);
	}
};

template <int D1, int D2>
SC_MODULE (sigmoid_activation) {

	// Ports
	sc_in<float>	input[D1][D2];
	sc_out<float>	output[D1][D2];

	// Operation
	void activate() {
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				float value = std::exp(input[row][col].read()) / (1 + std::exp(input[row][col].read()));
				output[row][col].write(value);
			}
		}
	}

	SC_CTOR (sigmoid_activation) {
		SC_METHOD (activate);
		dont_initialize();
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				sensitive << input[row][col];
			}
		}
	}
};

template <int D1, int D2>
SC_MODULE (array_multiplier) {

	// Ports
	sc_in<float>	input1[D1][D2];
	sc_in<float>	input2[D1][D2];
	sc_out<float>	output[D1][D2];

	// Operation
	void multiply() {
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				output[row][col].write(input1[row][col].read() * input2[row][col].read());
			}
		}
	}

	SC_CTOR (array_multiplier) {
		SC_METHOD (multiply);
			dont_initialize();
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				sensitive << input1[row][col];
				sensitive << input2[row][col];
			}
		}
	}
};

template <int D1, int D2>
SC_MODULE (array_subtractor) {

	// Ports
	sc_in<float>	input1[D1][D2];
	sc_in<float>	input2[D1][D2];
	sc_out<float>	output[D1][D2];

	// Operation
	void subtract() {
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				output[row][col].write(input1[row][col].read() - input2[row][col].read());
			}
		}
	}

	SC_CTOR (array_subtractor) {
		SC_METHOD (subtract);
			dont_initialize();
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				sensitive << input1[row][col];
				sensitive << input2[row][col];
			}
		}
	}
};

template <int D1, int D2>
SC_MODULE (matrix_transpose) {

	// Ports
	sc_in<float>	input[D1][D2];
	sc_out<float>	output[D1][D2];

	// Operation
	void transpose() {
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				output[row][col].write(input[col][row].read());
			}
		}
	}

	SC_CTOR (matrix_transpose) {
		SC_METHOD (transpose);
			dont_initialize();
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				sensitive << input[row][col];
			}
		}
	}
};

template <int D1, int D2>
SC_MODULE (ones_array) {

	// Ports
	sc_out<float>	output[D1][D2];

	// Operation
	void output () {
		for (int row = 0; row < D1; row++) {
			for (int col = 0; col < D2; col++) {
				output[row][col].write(1.0);
			}
		}
	}

	SC_CTOR (ones_array) {
		SC_METHOD (output);
	}
};

#endif /* SystemCMatrix.h */
