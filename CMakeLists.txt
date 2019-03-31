# This is just the version I have installed, not sure what I need
cmake_minimum_required(VERSION 3.14)


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

# First Library, Getopt!
# Note the CPP file in Getopt actually already has the folder name, so it gets access to the top level
# include directory!
add_library(Getopt "${SRC_DIR}/getopt/getopt_pp.cpp")
#target_include_directories(Getopt PRIVATE "${INCLUDE_DIR}")

# This is the main executable, SATSolver
add_executable(SATSolver "${SRC_DIR}/satsolver.cpp")
#target_include_directories(SATSolver PRIVATE "${INCLUDE_DIR}")

# Final Link Stage
target_link_libraries(
	SATSolver	# Main Function
	Getopt		# Argument Parsing
)