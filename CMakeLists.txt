# This is just the version I have installed, not sure what I need
cmake_minimum_required(VERSION 3.14)

# Project Name and Version Number
project("SATSolver"
	VERSION 0.1
	LANGUAGES CXX
	DESCRIPTION "Solves SAT Problems! What a Beast!"
)

# We are going to use at least 11 features!
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_STANDARD_REQUIRED ON)

# Don't include with the whole ../include/, just straight to the point
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")


# This is the executable and its linking and whatnot, currently we have written no libraries
add_executable(SATSolver "${SRC_DIR}/SATsolver.cpp")
target_link_libraries(SATSolver)