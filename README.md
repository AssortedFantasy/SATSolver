# ---------------------------------------------------
# Name : Jehanzeb Mirza
# ID: 1234567
# Name : Tharidu Witharana
# ID: 1534351
# CMPUT 275 , Winter 2019
#
# Final Project: SAT Solver
# ---------------------------------------------------

The program will read in boolean algebra expressions and test to see if the problem is satisfiable.
There are several components to this program: the SATSolver, the SATSimplifier, and the SAT Parser.

Included Files: 
  - build/
  - docs/
    - grammar.txt
    - Running Schema.txt
  - hist/
  - include/
    - getopt/
      - getopt_pp_standalone.h
      - getopt_pp.h
      - highlight.css
    - expression.h
    - mathCore.h
    - satform.h
    - satIO.h 
  - src/
    - getopt/
      - getopt_pp.cpp
    - mathCore/
      - mathCoreCompare.cpp
      - mathCoreConsturct.cpp
      - mathCoreSimplify.cpp
      - mathCoreStream.cpp
    - Parser/
      - parse.l
      - parse.y
      - MakeFile
    - expression.cpp
    - satform.cpp
    - satIO.cpp
    - satsolver.cpp 
  - README.MD
  - LICENSE
  - CMakeLists.txt

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

  