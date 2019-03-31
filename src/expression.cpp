#include "expression.h"

// Globally unique UUID, do not mess with this
int expression::global_uuid = 0;

expression::~expression(){
	for (expression* child : this->contents) {
		delete child;
	}
}
