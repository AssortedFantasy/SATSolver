#include "mathCore.h"
// Recursive Implementations of many Transforming Methods!


void mathCore::recursive_standardize(expression* a) {
	mathCore::standardize(a);
	// This has to be done after!
	for (auto child : a->contents) {
		mathCore::recursive_standardize(child);
	}
}


/*
	Children first, then ourselves
*/
void mathCore::recursive_combine_and(expression* a) {
	for (auto child : a->contents) {
		mathCore::recursive_combine_and(child);
	}
	mathCore::combine_and(a);
}

void mathCore::recursive_combine_or(expression* a) {
	for (auto child : a->contents) {
		mathCore::recursive_combine_or(child);
	}
	mathCore::combine_or(a);
}

void mathCore::recursive_combine_xor(expression* a) {
	for (auto child : a->contents) {
		mathCore::recursive_combine_xor(child);
	}
	mathCore::combine_xor(a);
}

void mathCore::recursive_combine_equiv(expression* a) {
	for (auto child : a->contents) {
		mathCore::recursive_combine_equiv(child);
	}
	mathCore::combine_equiv(a);
}
 
void mathCore::recursive_idempotent(expression* a) {
	mathCore::idempotent_law(a); // Guarentees either this is a Literal or Has no Literals
	for (auto child : a->contents) {
		mathCore::recursive_idempotent(child);
	}
	mathCore::universal_bound(a);
}


/*
	Evaluates things
	Passing in an evaluation multiset, which contains variables!
*/
void mathCore::evaluate(expression*a, expSet& evaluationSet) {
	// This is by nature a recursive call!
	for (auto child : a->contents) {
		evaluate(child, evaluationSet);
	}
	expSet::iterator iter;

	if (mathCore::is_var(a)) {
		if (evaluationSet.end() == (iter = evaluationSet.find(a))) {
			return;
		}
		else {
			if (is_negated(*iter)) {
				trans_false(a);
			}
			else {
				trans_true(a);
			}
		}
	}
};


