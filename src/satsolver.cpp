/*
	This is the main executable file, main is defined here
*/

#include <iostream>
#include <string>
#include "getoptpp/getopt_pp.h"
#include "expression.h"
#include "satIO.h"
#include "satform.h"

// Python Multi Line block strings don't exist in C unfortunately.
std::string help_string =
"SAT Solver, made by Jehanzeb Mirza and Tharidu Witharana!\n"
"\n"
"Options:\n"
"-h --help :\n"
"Displays a help message and exits the program.\n"
"- i --input 'Filename' :\n"
"Input File, use quotes for using spaces\n"
"Defaults to input.txt\n"
"- o --output 'Filename' :\n"
"Output File, use quotes for using spaces\n"
"Defaults to output.txt\n"
"- t --terminal :\n"
"Flag to terminal\n"
"- m --mode <integer between 0 and 2> :\n"
"One of a few modes!\n"
"0 : Satisfiability[Default]\n"
"1 : CNF, Transforms to Conjunctive Normal Form\n"
"2 : DNF, Transforms to Disjunctive Normal Form\n"
"3 : Unchanged, Expression just comes as expressed internally\n"
"4 : Unchanged, But the expressions are flattened to reduce parentheses\n"
"5 : Negated, In Reduced Standardized\n"
"6 : Dualed, In Reduced Standardized\n"
"7 : Standardized(Comprised Entirely of AND, OR, NOT)\n"
"8 : Reduced Standardized, Some Basic Simplifications done\n"
"9 : NANDIFY(Comprised Entirely of NAND)\n"
"10 : NORIFY(Comprised Entirely of NOR)\n";

/*
	Command Line Interface implementation for the project
	uses getoptpp which is a IO-Stream style Argument Parser.

	Options:
		-h --help:
			Displays a help message and exits the program.
		-i --input "Filename":
			Input File, use quotes for using spaces
			Defaults to input.txt
		-o --output "Filename":
			Output File, use quotes for using spaces
			Defaults to output.txt
		-t --terminal:
			Flag to terminal
		-m --mode <integer between 0 and 2>:
			One of a few modes!
	  0 : Satisfiability[Default]
		1 : CNF, Transforms to Conjunctive Normal Form
		2 : DNF, Transforms to Disjunctive Normal Form
		3 : Unchanged, Expression just comes as expressed internally
		4 : Unchanged, But the expressions are flattened to reduce parentheses
		5 : Negated
		6 : Dualed
		7 : Standardized(Comprised Entirely of AND, OR, NOT)
		8 : NANDIFY(Comprised Entirely of NAND)
		9 : NORIFY(Comprised Entirely of NOR)
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

#ifdef DEBUG_INPUT_PARSE
	std::cout << "Terminal Mode: " << terminal_flag << "\n"
		<< "Input File String: " << input_file_string << "\n"
		<< "Output File String: " << output_file_string << "\n"
		<< "Mode: " << mode << "\n";
	std::cin.get();
#endif // DEBUG_INPUT_PARSE

	// Read in from the input file
	expression* in_exp = read_exp_from_file(input_file_string);
	expression* out_exp = NULL;
	
	if (in_exp == NULL) {
		std::cout << "Had problems reading input expression!\n";
		return 1;
	}

	/*
		Convert the input expression into an output expression,
		These are all implemented in satform
	*/
	switch (mode) {
	case 0:
		std::cout << "Satisfyability Not Implemented\n";
		break;
	case 1:
		CNF_FORM(in_exp);
		out_exp = in_exp;
		break;
	case 2:
		DNF_FORM(in_exp);
		out_exp = in_exp;
		break;
	case 3:
		reduce_associative(in_exp);
		out_exp = in_exp;
		break;
	case 4:
		out_exp = in_exp;
		break;
	case 5:
		negated_standard(in_exp);
		out_exp = in_exp;
		break;
	case 6:
		dual_standard(in_exp);
		out_exp = in_exp;
		break;
	case 7:
		standard_form_raw(in_exp);
		out_exp = in_exp;
		break;
	case 8:
		standard_form(in_exp);
		out_exp = in_exp;
		break;
	case 9:
		std::cout << "NANDIFY Not Implemented\n";
		break;
	case 10:
		std::cout << "NORIFY Not Implemented\n"; 
		break;
	default:
		std::cout << "Invalid Mode! Use -h or --help for more information!\n";
		return 1;
	}

	if (out_exp == NULL) {
		std::cout << "Had problems creating output expression!\n";
		return 1;
	}

	write_exp_to_file(out_exp, output_file_string, terminal_flag);

	std::cin.get();
	return 0;
}
