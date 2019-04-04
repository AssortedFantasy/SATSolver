#include "mathCore.h"
// Recursive Implementations of many Transforming Methods!


void mathCore::recursive_standardize(expression* a) {
	mathCore::standardize(a);
	// This has to be done after!
	for (auto child : a->contents) {
		mathCore::standardize(a);
	}
}


/*
	Children first, then ourselves
*/
void mathCore::recursive_combine_and(expression* a) {
	for (auto child : a->contents) {
		mathCore::combine_and(a);
	}
	mathCore::combine_and(a);
}

void mathCore::recursive_combine_or(expression* a) {
	for (auto child : a->contents) {
		mathCore::combine_or(a);
	}
	mathCore::combine_or(a);
}

void mathCore::recursive_combine_xor(expression* a) {
	for (auto child : a->contents) {
		mathCore::combine_xor(a);
	}
	mathCore::combine_xor(a);
}

void mathCore::recursive_combine_equiv(expression* a) {
	for (auto child : a->contents) {
		mathCore::combine_equiv(a);
	}
	mathCore::combine_equiv(a);
}
 