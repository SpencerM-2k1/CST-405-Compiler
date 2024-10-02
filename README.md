# CST-405-Compiler
Written by Evan Lloyd and Spencer Meren

## Compilation and Execution

Bison and Flex must be installed in order to compile this program.

A Makefile is provided to easily compile and execute the parser.

- `make test1`, `make test2`, `make test3` will compile and execute the program with a specific test program as a launch argument. Each test corresponds to a test program located in `/samples`. After execution, output logs, TACs, and the compiled MIPS code for the test program of choice will be located in `/outputs`
- `make clean` will delete all executables, object files, and output file