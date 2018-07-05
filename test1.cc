#include <iostream>
#include <systemc>

#include "SystemCMatrix.h"
#include "Templates.h"

int sc_main(int argc, char* argvp[]) {

    MAT_MULT(input, output, 1, 10)

    return 0;
}