# ---------------------------------------------------
# Name: Jehanzeb Mirza
# ID: 1539833
# Name: Tharidu Witharana
# ID: 1534351
#
# CMPUT 275, Winter 2019
# Final Project: SAT Solver
# ---------------------------------------------------

The program will read in boolean algebra expressions and test to see if the problem is satisfiable.
There are several components to this program: the SATSolver, the SATSimplifier, and the SAT Parser.

Included Files: 
  - SatSolver.exe // This is a pre-compiled Windowsx64 binary
  - docs/
    - grammar.txt
    - Running Schema.txt
  - hist/
    - boolean_grammar_specification
    - expression_public_versions.h
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

Pre-requisites:
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

Operation Modes:
  1: Conjuctive Normal Form: Outputs the minterms of a boolean expression
  2: Disjunctive Normal Form: Outputs the maxterms of a boolean expression
  3: Raw Parse: Outputs the expression as it was read in without simplification
  4: Flattened Parse: Associativity is used to remove unnecessary brackets
  5: Negated: Outputs the negation of the expression that was read in
  6: Dualed: Outputs the dualed expression of the one read in
  7: Standardized: Converts expression to use only AND, OR, and NOT
  8: Standardized Reduced: Reduces the standardized expression to a simpler form

  Invalid modes will print an error message to the user


Notes and Assumptions:
  The user interacts with the program through text files and a terminal.
  The input expression must be written into input.txt, and obey the grammar outlined in grammar.txt
  The output expression will be appended to output.txt
  The Parser will each step of the tokenizing process if the DEBUG_INPUT_PARSE flag is defined in cmake

  The main function is located in satsolver.cpp, which first calls satIO to read and parse the input epxression. Next, if input was read, it determines what operations to do on the expression. All the reduction routines occur in satform.cpp, which calls specific reduction techniques in order to produced the requested output.
  All of these reduction operations use boolean algebra laws that are defined in the various parts of mathCore. These reductions occur recursively to ensure the entire expression is properly reduced. Afterwards, if input was successfully processed, it uses satIO to write the expression to the output.txt file. If the expression was not processed properly, it prints an error.

  To modify the parser and lexer, one must modify the parse.l and parse.y files.
  If the parser and lexer are modified, the makefile in the Parser directory must be run. The makefile has several targets:
    - all: builds the parser and lexer files and makes them accessible to the cmake makefile
    - clean: removes all the processed lexer and parser files
    - flex: builds the lexer files
    - bison: builds parser files
    - compile_test: builds the lexer and parser files, then compiles and links them into a test binary in the Parser source directory
  Note that the bison and flex generate c-files which must be later compiled and linked separately. The cmake build file uses these generated c-files
  If the local makefile is not run before building with the cmake makefile, the changes to the parser and lexer will not be used. 

Acknowledgements:
  Creators of GetOptpp
    Daniel Gutson
  Creator of Bison
    Robert Corbett
  Creators of Flex
    Mike Lesk
    Eric Schmidt

  
                            DESIGN FLOWCHART

              +--------+      +--------+
              |        |      |        |         +--------+
      +------>+  USER  +----->+  MAIN  +<------->+ GETOPT |
      |       |        |      |        |         +--------+
      |       +------+-+      +-----+--+
      |              |              |
      |              v              v
+-----+------+   +---+-------+  +---+---+         +---------+
| OUTPUT.TXT |   | INPUT.TXT |  | SATIO +<------->+ SATFORM +<----+
+------+-----+   +-----+-----+  ++-+--+-+         +---------+     |
       ^               |         | |  ^                           |
       |               |         | |  +---------+                 |
       +<------------------------+ v            |                 |
                       |        +--+----+    +--+-----+   +-------v--+
                       +------->+ LEXER +----> PARSER |   | MATHCORE |
                                +-------+    +----+---+   +-+--------+
                                                  ^         ^
                                                  |         |
                                                +-+---------++
                                                | EXPRESSION |
                                                +------------+
1. USER writes to INPUT.TXT 
2. USER communicates commands to MAIN
3. GETOPT is used to interpret User Commands
4. MAIN calls SATIO, passing in arguments from GETOPT
5. SATIO Calls the Lexer
6. The LEXER reads in from INPUT.TXT
7. The LEXER outputs tokens to the Parser
8. The PARSER converts the tokens to EXPRESSIONS
9. The PARSER returns the EXPRESSION to SATIO
10. SATIO calls the appropriate SATFORM function based upon GETOPT arguments, passing in the EXPRESSION
11. SATFORM calls MATHCORE functions and passes in the EXPRESSION
12. MATHCORE does the appropriate reductions on the EXPRESSION and returns it to SATFORM
13. SATFORM returns the expression to SATIO
14. SATIO appends the expression to OUTPUT.TXT
15. The USER reads result from OUTPUT.TXT
