### Description
This is a synthesizable, single layer artificial neural network implemented using the SystemC framework.

### Running
```
make
./main.exe
```

### Notes on Implementation



### Notes on Problems Faced

+ **Problem: Memory usage exceeding amount allowed on stack.** Because of the size of the matrices demanded by the specifications and implementation of this program, running the program would consistently lead to segmentation faults.
+ **Solution 1: (Currently in use)** To mitigate this problem, `-Wl,-stack_size -Wl,10000000` was passes to the linker, to increase the maximum size allowed in the stack. It is not yet forseen whether this will prove to be a problem when implemented on a FPGA, as the limitations of the FPGAs used are not yet available.
+ **Solution 2:** While not currently in use, one could instead reduce the BATCH_SIZE used by the program, in the case that one cannot physically supply any more space for a FPGA of the desired BATCH_SIZE.

+ **Problem: Templatized SC_MODULE naming behavior.** When implementing a header file for a templatized SC_MODULE, the symbol names of the internal methods are not easily accessible, and so causes issues when linking an external object file with the internal methods for the templatized SC_MODULE.
+ **Solution:** Until a fix to this issue is found, all functions are implemented in-line in the header file of SC_MODULEs.

### Notes on Methods Used



### Notes on Overall Structure



### Sources Used in Implementation

+ SystemC Tutorial [http://www.asic-world.com/systemc/tutorial.html]
+ Backpropogation Reference [https://mattmazur.com/2015/03/17/a-step-by-step-backpropagation-example/]
