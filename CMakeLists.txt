# This is just the version I have installed, not sure what I need
cmake_minimum_required(VERSION 3.12)


# Project Name and Version Number
project(
  "SATSolver"
  VERSION 0.6
  LANGUAGES CXX C
  DESCRIPTION "Solves SAT Problems! What a Beast!"
)

# debug ones
add_compile_definitions(DEBUG_INPUT_PARSE)


# We are going to use 11 features!
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_STANDARD_REQUIRED ON)

# Don't include with the whole ../include/, just straight to the point
# Make sure you add a corresponding target_include_directories for each target! (Mabye?)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# This works badly with the autofill, so lets not do this
#include_directories("${INCLUDE_DIR}")

# Getopt
add_library(Getopt "${SRC_DIR}/getopt/getopt_pp.cpp")
target_include_directories(Getopt PRIVATE "${INCLUDE_DIR}")

# Expression Functionality
add_library(Expressions "${SRC_DIR}/expression.cpp")
target_include_directories(Expressions PRIVATE "${INCLUDE_DIR}")

# Math Core
# Multiple.cpp files
set(MATHCORECODE 
  "${SRC_DIR}/mathCore/mathCoreCompare.cpp"
  "${SRC_DIR}/mathCore/mathCoreConstruct.cpp"
  "${SRC_DIR}/mathCore/mathCoreStream.cpp"
  "${SRC_DIR}/mathCore/mathCoreSimplify.cpp"
  "${SRC_DIR}/mathCore/mathCoreRecursive.cpp"
)
add_library(MathCore ${MATHCORECODE})
target_include_directories(MathCore PRIVATE "${INCLUDE_DIR}")

# Parser
set(PARSERCODE
  "${SRC_DIR}/Parser/lex.yy.cc"
  "${SRC_DIR}/Parser/parse.tab.cc"
)
add_library(Parser ${PARSERCODE})
target_include_directories(Parser PRIVATE "${INCLUDE_DIR}")

# SATIO
add_library(SATIO "${SRC_DIR}/satIO.cpp")
target_include_directories(SATIO PRIVATE "${INCLUDE_DIR}")

# SATFORM
add_library(SATFORM "${SRC_DIR}/satform.cpp")
target_include_directories(SATFORM PRIVATE "${INCLUDE_DIR}")


# This is the main executable, SATSolver
add_executable(SATSolver "${SRC_DIR}/satsolver.cpp")
target_include_directories(SATSolver PRIVATE "${INCLUDE_DIR}")


# Final Linking, GCC is very picky so you have to be careful with ordering
target_link_libraries(
  SATSolver	# Main Function                     ALL

  SATIO     # File Input OutPut                 Parser and MathCore
  SATFORM   # Modifying the Expression Trees    MathCore
  

  Parser      # Lexer and Parser                Both MathCore and Expressions
  MathCore    # Most Real Expression Methods    Expressions
  Expressions # Expression Methods              NO DEPENDENCIES
  Getopt      # Argument Parsing                NO DEPENDENCIES
)