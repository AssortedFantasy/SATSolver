/*
	This is the main executable file, main is defined here
*/


#include <iostream>
#include <string>
#include "..\include\getoptpp\getopt_pp.h"	// Temporary, so Autocomplete works
// #include "getoptpp\getopt_pp.h"
// #include "expression.h"

// Python Multi Line block strings don't exist in C unfortunately.
std::string help_string =
"SAT Solver, made by Jehanzeb Mirza and Tharidu Witharana\n"
"\n"
"Options:\n"
"-h --help :\n"
"	Displays a help message and exits the program.\n"
"	- i --input 'Filename' :\n"
"	Input File, use quotes for using spaces(not proven ? )\n"
"	Defaults to input.txt\n"
"	- o --output 'Filename' :\n"
"	Output File, use quotes for using spaces(not proven ? )\n"
"	Defaults to output.txt\n"
"	- t --terminal :\n"
"	Flag to terminal\n"
"	- m --mode <integer between 0 and 2> :\n"
"	One of a few modes!\n"
"	0[Default] : Satisfiability\n"
"	1 : CNF, Transforms to Conjunctive Normal Form\n"
"	2 : DNF, Transforms to Disjunctive Normal Form\n";

/*
	Command Line Interface implementation for the project
	uses getoptpp which is a IO-Stream style Argument Parser.

	Options:
		-h --help:
			Displays a help message and exits the program.
		-i --input "Filename":
			Input File, use quotes for using spaces (not proven?)
			Defaults to input.txt
		-o --output "Filename":
			Output File, use quotes for using spaces (not proven?)
			Defaults to output.txt
		-t --terminal:
			Flag to terminal
		-m --mode <integer between 0 and 2>:
			One of a few modes!
			0 [Default]: Satisfiability
			1		   : CNF, Transforms to Conjunctive Normal Form
			2		   : DNF, Transforms to Disjunctive Normal Form
*/
int main(int argc, char* argv[]){
	GetOpt::GetOpt_pp optparser(argc, argv);
	std::string input_file_string = "input.txt",
		output_file_string = "output.txt";
	bool terminal_flag;
	int mode = 0;

	// If the help flag is set, then we don't actually do any work!
	if (optparser >> GetOpt::OptionPresent('h', "help")) {
		std::cout << help_string;
		return 0;
	}

	// Not providing the option should leave input file strings alone
	// Gonna do some testing now
	optparser >> GetOpt::OptionPresent('t', "terminal", terminal_flag)
		>> GetOpt::Option('i', "input", input_file_string)
		>> GetOpt::Option('o', "output", output_file_string)
		>> GetOpt::Option('m', "mode", mode);

	// Debug Parsing
	std::cout << "Terminal Mode: " << terminal_flag << "\n"
		<< "Input File String: " << input_file_string << "\n"
		<< "Output File String: " << output_file_string << "\n"
		<< "Mode: " << mode << "\n";
	std::cin.get();

	return 0;
}
