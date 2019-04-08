#include "satform.h"

void reduce_associative(expression* a) {
	// Reduce the associative Components of this
	mathCore::recursive_combine_and(a);
	mathCore::recursive_combine_or(a);
	mathCore::recursive_combine_xor(a);
	mathCore::recursive_combine_equiv(a);
}


/*
	Everything is AND, OR and Negated Variables
*/
void standard_form_raw(expression * a) {
	reduce_associative(a);
	mathCore::recursive_standardize(a);
	// Now you should be able to do this well
	mathCore::combine_and(a);
	mathCore::combine_or(a);
}


/*
In reduced form
*/
void standard_form(expression* a) {
	reduce_associative(a);
	mathCore::recursive_standardize(a);
	// Now you should be able to do this well
	mathCore::combine_and(a);
	mathCore::combine_or(a);
	mathCore::recursive_idempotent(a);
}


void dual_standard(expression * a) {
	mathCore::dual(a);
	standard_form(a);
}

void negated_standard(expression* a) {
	mathCore::negate(a);
	standard_form(a);
}

void CNF_FORM(expression* a) {
	standard_form(a);
	mathCore::to_CNF(a);
}

void DNF_FORM(expression* a) {
	standard_form(a);
	mathCore::to_DNF(a);
}