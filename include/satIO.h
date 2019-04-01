#ifndef _SAT_IO_H
#define _SAT_IO_H

#include <string>
#include "expression.h"

// I/O Functions which main uses

/*
	Reads an expression from a file
	returns NULL if an error occurs
*/
expression* read_exp_from_file(std::string& filename);

/*
	Writes an expression to a file
	returns true if written successfully
	
	Terminal Flag makes it so it prints to stdout as well
*/
bool write_exp_to_file(expression* exp, std::string& filename, bool terminal);

#endif