#include "satIO.h"

// Attempts to parse a file into an expression
// Returns NULL on failure
expression* read_exp_from_file(std::string& filename){
  expression* input_expression = parse_expression(filename.c_str());
  return input_expression;
}

// Attempts to parse a file into an expression
bool write_exp_to_file(expression* exp, std::string& filename, bool terminal) {
  std::string exp_as_string = mathCore::as_string(exp);
  bool success;

  if (terminal) {
    std::cout << "\n\n\nOutputExpression:\n" << exp_as_string << "\n\n\n";
  }

  std::ofstream outputfile(filename, std::ios::out | std::ios::app);
  outputfile << exp_as_string << "\n\n\n";
  success = outputfile.good();
  outputfile.close();
  return success;
}