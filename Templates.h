#ifndef _TEMPLATES_H_
#define _TEMPLATES_H_

#define MAT_MULT(MODULE_NAME, TRACE_NAME, INPUT, OUTPUT, D1, D2) \
    sc_signal<float>    INPUT[D1][D2];  \
    sc_signal<float>    OUTPUT[D1][D2]; \
	sigmoid_activation<D1,D2> MODULE_NAME(#MODULE_NAME); \
	for (int row = 0; row < D1; row++) {    \
			for (int col = 0; col < D2; col++) {    \
				MODULE_NAME.INPUT[row][col](INPUT[row][col]);  \
				MODULE_NAME.OUTPUT[row][col](OUPUT[row][col]);    \
			}   \
	}   \
	sc_trace_file *TRACE_NAME = sc_create_vcd_trace_file(#TRACE_NAME); \
	for (int row = 0; row < D1; row++) {	\
			for (int col = 0; col < D2; col++) {	\
				sc_trace(TRACE_NAME , INPUT[row][col], #INPUT);	\
				sc_trace(TRACE_NAME , OUTPUT[row][col], #OUTPUT);	\
			}	\
	}

MAT_MULT(test_module,trace_file,input,output,2,2)

#endif /* Templates.h */