#include "mathCore.h"

// Returns True if it is a literal value
bool mathCore::is_literal(expression* a) {
	return a->FLAGS & F_LIT;
}

bool mathCore::is_var(expression* a) {
	return a->FLAGS & F_VAR;
}

bool mathCore::is_negated(expression* a) {
	return a->FLAGS & F_NEG;
}

bool mathCore::is_dualed(expression* a) {
	return a->FLAGS & F_DUAL;
}

bool mathCore::is_and(expression* a) {
	return a->FLAGS & F_AND;
}

bool mathCore::is_or(expression* a) {
	return a->FLAGS & F_OR;
}

bool mathCore::is_xor(expression* a) {
	return a->FLAGS & F_XOR;
}

bool mathCore::is_imply(expression* a) {
	return a->FLAGS & F_IMPLY;
}

bool mathCore::is_equiv(expression* a) {
	return a->FLAGS & F_EQUIV;
}

// Check if two variables are the same, they might be negations of each other
bool mathCore::var_equal(expression* a, expression* b) {
	if (mathCore::is_var(a) && mathCore::is_var(b)) {
		return (a->uuid == b->uuid);
	}
	return false;
}

// A product of literals or variables! or just a literal / variable
bool mathCore::is_product(expression* a) {
	if (is_var(a) || is_literal(a)) { // Single literals/variables are products
		return true;
	}
	else if (is_and(a)) { // Products must be ands
		for (auto child : a->contents) {
			if (!( is_product(child))) { // Products must be ands of products
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

bool mathCore::is_sum(expression* a) {
	if (is_var(a) || is_literal(a)) { // Single literals/variables are sums
		return true;
	}
	else if (is_or(a)) { // Sums must be ors
		for (auto child : a->contents) {
			if (!(is_sum(child))) { // Products must be ors of sums
				return false;
			}
		}
		return true;
	}
	else {
		return false;
	}
}

bool mathCore::is_CNF(expression* a) {
	// Conjunctive normal form, AKA product of sums
	if (is_var(a) || is_literal(a)) {
		return true;					// Again, just a literal is a CNF
	}
	else if (is_and(a)) {
		for (auto child : a->contents) {
			if (!(is_sum(child))) {		// All children must be sums
				return false;
			}
		}
		return true;	// All Children were sums!
	}
	else {
		return false;
	}
}

bool mathCore::is_DNF(expression* a) {
	// Conjunctive normal form, AKA sum of products
	if (is_var(a) || is_literal(a)) {
		return true;					// Again, just a literal is a DNF
	}
	else if (is_or(a)) {					// Must be a sum
		for (auto child : a->contents) {
			if (!(is_product(child))) {		// All children must be sums
				return false;
			}
		}
		return true;	// All Children were sums!
	}
	else {
		return false;
	}
}