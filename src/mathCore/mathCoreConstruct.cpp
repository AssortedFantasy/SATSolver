#include "mathCore.h"

// Constructs a literal expression
// The literal expression is literal true!
// All literals have uuid 1!
expression* mathCore::literal_true() {
	expression* exp = new expression;
	exp->FLAGS = F_LIT;
	exp->uuid = 1;
	return exp;
}

// Constructs literal false
expression* mathCore::literal_false() {
	expression* exp = mathCore::literal_true();
	mathCore::negate(exp);
	return exp;
}

// Constructs a literal expression out of a boolean
expression* mathCore::literal(const bool val) {
	if (val) {
		return mathCore::literal_true();
	}
	else {
		return mathCore::literal_false();
	}
}

// Constructs a variable expression
expression* mathCore::variable(const std::string& identifier) {
	expression* exp = new expression;
	exp->FLAGS = F_VAR;

	auto iter = expression::string_to_uuid.find(identifier);

	if (iter != expression::string_to_uuid.end()) {
		// Already is an existant identifier
		exp->uuid = iter->second;
	}
	else {
		// This is a new identifier
		exp->uuid = expression::global_uuid;

		// Make sure to save it in the map
		expression::uuid_to_string[exp->uuid] = identifier;
		expression::string_to_uuid[identifier] = exp->uuid;
		
		// Next literal gets the next uuid up
		expression::global_uuid++;
	}
	return exp;
}

expression* mathCore::construct_global_var() {
	expression* exp = new expression;
	exp->FLAGS = F_VAR;
	exp->uuid = 2;
	return exp;
}

// From UUID
expression* mathCore::varFromUUID(unsigned int uuid) {
	expression* exp = new expression;
	exp->FLAGS = F_VAR;

	// Bad things are happening if this happens, only valid UUIDs can be used
	if (uuid >= expression::global_uuid || uuid < 2) {
		throw;
	}
	exp->uuid = uuid;
	return exp;
}


// Delete all the children from a expression!
void mathCore::delete_children(expression* a) {
	for (auto child : a->contents) {
		delete child;
	}
}

// Negate an Expression
void mathCore::negate(expression* a){
	a->FLAGS ^= F_NEG;
}

// Dual an Expression
void mathCore::dual(expression* a) {
	if (mathCore::is_literal(a)) {
		// Dual of a Literal is its negation
		mathCore::negate(a);
	}
	else if (!mathCore::is_var(a)){
		// If its not a varible you flip its dual
		a->FLAGS ^= F_DUAL;
	}
}

// Unduals an Expression, Nothing if its already fine
void mathCore::un_dual(expression* a) {
	a->FLAGS &= ~(F_DUAL);
}

// Unduals an Expression, Nothing if its already fine
void mathCore::un_negate(expression* a) {
	a->FLAGS &= ~(F_NEG);
}


// Change to AND OR XOR EQUIV VAR TRUE FALSE
void mathCore::trans_and(expression* a) {
	mathCore::clear_type_flag(a);
	a->FLAGS |= F_AND;
};

void mathCore::trans_or(expression* a) {
	mathCore::clear_type_flag(a); 
	a->FLAGS |= F_OR;
};

void mathCore::trans_xor(expression* a) {
	mathCore::clear_type_flag(a); 
	a->FLAGS |= F_XOR;
};

void mathCore::trans_equiv(expression* a) {
	mathCore::clear_type_flag(a); 
	a->FLAGS |= F_EQUIV;
};

void mathCore::trans_imply(expression* a) {
	mathCore::clear_type_flag(a); 
	a->FLAGS |= F_IMPLY;
};


void mathCore::trans_var(expression* a, unsigned int uuid) {
	mathCore::clear_type_flag(a);
	a->FLAGS |= F_VAR;
	a->uuid = uuid;

	// Invalid Variable, this never happens so it gets branch predicted over
	if (uuid < 2 || uuid > expression::global_uuid) {
		throw;
	}
};


void mathCore::trans_true(expression* a) {
	mathCore::clear_type_flag(a);
	mathCore::un_negate(a);
	mathCore::un_dual(a);
	a->FLAGS |= F_LIT;
};


void mathCore::trans_false(expression* a) {
	mathCore::trans_true(a);
	mathCore::negate(a);
};


void mathCore::trans_lit(expression* a, bool val) {
	if (val) {
		mathCore::trans_true(a);
	}
	else {
		mathCore::trans_false(a);
	}
}


// Private Method!
// Does not change dual or negated state!
void mathCore::clear_type_flag(expression* a) {
	a->FLAGS &= F_NEG | F_DUAL;
}

//AND OR XOR EQUIV all have basially the same code
expression* mathCore::binary_and(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_AND;
	exp->contents.insert(a);
	exp->contents.insert(b);
	return exp;
}

expression* mathCore::binary_or(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_OR;
	exp->contents.insert(a);
	exp->contents.insert(b);
	return exp;
}

expression* mathCore::binary_xor(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_XOR;
	exp->contents.insert(a);
	exp->contents.insert(b);
	return exp;
}

expression* mathCore::binary_equiv(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_EQUIV;
	exp->contents.insert(a);
	exp->contents.insert(b);
	return exp;
}

/* DEPRECATED - MULTICONSTRUCTORS
These Multi things aren't really used, so I have deprecated them to reduce workload and maintainance


// Multi Versions of Above, Essentially we just copy construct the vector
expression* mathCore::multi_and(const std::multiset<expression*>& children){
	expression* exp = new expression;
	exp->FLAGS = F_AND;
	exp->contents = children;
	return exp;
}

expression* mathCore::multi_or(const std::multiset<expression*>& children) {
	expression* exp = new expression;
	exp->FLAGS = F_OR;
	exp->contents = children;
	return exp;
}

expression* mathCore::multi_xor(const std::multiset<expression*>& children) {
	expression* exp = new expression;
	exp->FLAGS = F_XOR;
	exp->contents = children;
	return exp;
}

expression* mathCore::multi_equiv(const std::multiset<expression*>& children) {
	expression* exp = new expression;
	exp->FLAGS = F_EQUIV;
	exp->contents = children;
	return exp;
}
*/

// Impication is the same as the binaries, but is exclusivally binary
expression* mathCore::imply(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_IMPLY;
	exp->contents.insert(a);
	exp->contents.insert(b);
	return exp;
}

// Recursively Copies, Is quite hard on the memory usage so be careful with it
expression* mathCore::copy(expression * a) {
	expression* exp = new expression;
	
	// Copy flags and UUID
	exp->FLAGS = a->FLAGS;
	exp->uuid = a->uuid;

	// Copy over there contents, as a deep copy
	if (a->contents.size()) {
		auto hint = exp->contents.begin();
		for (auto iter = a->contents.begin(); iter != a->contents.end(); iter++) {
			// Hints are powerful, when strictly ordered, the inserting takes constant
			// instead of logirthmic time, in this case that makes this a linear time copy.
			hint = exp->contents.insert(hint, mathCore::copy(*iter));
		}
	}
	return exp;
}

expression* mathCore::global_literal = mathCore::literal_true();
expression* mathCore::global_variable = mathCore::construct_global_var();
