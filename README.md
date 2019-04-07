# SATSolver
A SAT Solver coded up in C++, Final Project for CMPUT 275 Winter 2019
Written By: Jehanzeb Mirza and Tharidu Witharana

The program will read in boolean algebra expressions and test to see if the problem is satisfiable.
There are several components to this program: the SATSolver, the SATSimplifier, and the SAT Parser.

Pre-requities:
  CMake Version 3.12 or higher is required.
  Compiler compatible with C++11 standard

Running Instructions:
  1. From within the build folder, run cmake command on the root directory
  2. Run the make from within the build directory
  3. Create an input.txt file in the build directory
     Consult the grammar.txt file in the docs folder for the input syntax.
  4. Execute the SatSolver binary in the desired mode,
     Output will be appended to the output.txt file

Command Line arguments:
  - 'h' for help menu
  - 'm' for mode
  - 'i' for input
  - 'o' for output
  - 't' for terminal output 

Acknowledgements:
  Creators of GetOptpp
    Daniel Gutson
  Creator of Bison
    Robert Corbett
  Creators of Flex
    Mike Lesk
    Eric Schmidt

  