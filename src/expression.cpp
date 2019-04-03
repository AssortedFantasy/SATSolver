#include "expression.h"

// Globally unique UUID, do not mess with this
// Also globally unique maps going back and forth!
unsigned int expression::global_uuid = 0;
std::unordered_map<unsigned int, std::string> expression::uuid_to_string;
std::unordered_map<std::string, unsigned int> expression::string_to_uuid;

expression::~expression(){
	for (expression* child : this->contents) {
		delete child;
	}
}

// Empty Constructor, needed because its private
expression::expression() {};