# This is just the version I have installed, not sure what I need
cmake_minimum_required(VERSION 3.10.2)


# Project Name and Version Number
project(
	"SATSolver"
	VERSION 0.1
	LANGUAGES CXX
	DESCRIPTION "Solves SAT Problems! What a Beast!"
)


# We are going to use at least 11 features!
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_STANDARD_REQUIRED ON)

# Don't include with the whole ../include/, just straight to the point
# Make sure you add a corresponding target_include_directories for each target! (Mabye?)
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${INCLUDE_DIR}")

# Getopt
add_library(Getopt "${SRC_DIR}/getopt/getopt_pp.cpp")
# Expression Functionality
add_library(Expressions "${SRC_DIR}/expression.cpp")
# Math Core
add_library(MathCore "${SRC_DIR}/mathCore.cpp")

# This is the main executable, SATSolver
add_executable(SATSolver "${SRC_DIR}/satsolver.cpp")

# Final Link Stage
target_link_libraries(
	SATSolver	# Main Function
	Getopt		# Argument Parsing
	Expressions # Expression Methods
	mathCore	# Most Real Expression Methods
)
