#include "expression.h"

// Globally unique UUID, do not mess with this
// All literals have UUID 1, all variables have UUID >=2
// Also globally unique maps going back and forth!
unsigned int expression::global_uuid = 3;
std::unordered_map<unsigned int, std::string> expression::uuid_to_string;
std::unordered_map<std::string, unsigned int> expression::string_to_uuid;


expression::~expression(){
	for (expression* child : this->contents) {
		delete child;
	}
}

// Empty Constructor, needed because its private
expression::expression() {};

// Compare UUID
bool compareUUID::operator()(const expression* const a, const expression* const b) const {
	return (a->uuid) < (b->uuid);
}
