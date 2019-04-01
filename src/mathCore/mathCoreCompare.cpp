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