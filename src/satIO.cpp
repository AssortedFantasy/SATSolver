#include "satIO.h"

/*TODO: THARIDU Write this*/
expression* read_exp_from_file(std::string& filename){
	expression* input_expression = parse_expression(filename.c_str);
	return input_expression;
}


bool write_exp_to_file(expression* exp, std::string& filename, bool terminal) {
	
	return true;
}