#include "mathCore.h"

// Constructs a literal expression
// The literal expression is literal true!
expression* mathCore::literal_true() {
	expression* exp = new expression;
	exp->FLAGS = F_LIT;
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

//AND OR XOR EQUIV all have basially the same code
expression* mathCore::binary_and(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_AND;
	exp->contents.reserve(2); // Space for two
	exp->contents.emplace_back(a);
	exp->contents.emplace_back(b);
	return exp;
}

expression* mathCore::binary_or(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_OR;
	exp->contents.reserve(2); // Space for two
	exp->contents.emplace_back(a);
	exp->contents.emplace_back(b);
	return exp;
}

expression* mathCore::binary_xor(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_XOR;
	exp->contents.reserve(2); // Space for two
	exp->contents.emplace_back(a);
	exp->contents.emplace_back(b);
	return exp;
}

expression* mathCore::binary_equiv(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_EQUIV;
	exp->contents.reserve(2); // Space for two
	exp->contents.emplace_back(a);
	exp->contents.emplace_back(b);
	return exp;
}

// Multi Versions of Above, Essentially we just copy construct the vector
expression* mathCore::multi_and(const std::vector<expression*>& children){
	expression* exp = new expression;
	exp->FLAGS = F_AND;
	exp->contents = children;
	return exp;
}

expression* mathCore::multi_or(const std::vector<expression*>& children) {
	expression* exp = new expression;
	exp->FLAGS = F_OR;
	exp->contents = children;
	return exp;
}

expression* mathCore::multi_xor(const std::vector<expression*>& children) {
	expression* exp = new expression;
	exp->FLAGS = F_XOR;
	exp->contents = children;
	return exp;
}

expression* mathCore::multi_equiv(const std::vector<expression*>& children) {
	expression* exp = new expression;
	exp->FLAGS = F_EQUIV;
	exp->contents = children;
	return exp;
}

// Impication is the same as the binaries, but is exclusivally binary
expression* mathCore::imply(expression* a, expression* b) {
	expression* exp = new expression;
	exp->FLAGS = F_IMPLY;
	exp->contents.reserve(2); // Space for two
	exp->contents.push_back(a);
	exp->contents.push_back(b);
	return exp;
}
