#include "mmult_temp.h"

template <int M1, int S, int M2>
void mmult_temp<M1, S, M2>::clear_temp() {
	for (int row = 0; row < M1; row++) {
		for (int col = 0; col < M2; col++) {
			temp[row][col] = 0;
		}
	}
}

template <int M1, int S, int M2>
void mmult_temp<M1, S, M2>::run() {
	clear_temp();
	// Filling Temp
	for (int i = 0; i < M1; i++) {
	for (int j = 0; j < M2; j++) {
	for (int k = 0; k < S; k++) {
		temp[i][j] += input1[i][k].read() * input2[k][j].read();
	}}}
	// Sending Temp to Output
	for (int row = 0; row < M1; row++) {
		for (int col = 0; col < M2; col++) {
			output[row][col].write(temp[row][col]);
		}
	}
}
