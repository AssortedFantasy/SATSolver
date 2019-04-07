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
 

#include <iostream>



void mathCore::recursive_idempotent(expression* a) {
	mathCore::idempotent_law(a);
	mathCore::universal_bound(a);

	// We simplify first, because it might help stop recursion

	expSet tempSet;
	expression* child;
	auto iter = a->contents.begin();

	
	while (iter != a->contents.end()) {
		child = *iter;
		iter = a->contents.erase(iter);

		recursive_idempotent(child);
		tempSet.insert(child);
	}

	mergeMultiSet(a->contents, tempSet);

	mathCore::idempotent_law(a);
	mathCore::universal_bound(a);

	// We simplify After, because one of our children might have changed!
}


/*
	Evaluates things
	Passing in an evaluation multiset, which contains variables!
*/
void mathCore::evaluate(expression*a, expSet& evaluationSet) {
	// This is by nature a recursive call!
	// This is a transmuting function call though!
	expSet tempSet;
	expression* child;
	auto iter = a->contents.begin();
	while (iter != a->contents.end()) {
		child = *iter;
		iter = a->contents.erase(iter);

		evaluate(child, evaluationSet);
		tempSet.insert(child);
	}
	mergeMultiSet(a->contents, tempSet);

	// Transmutes variables into literals if they are in the evaluation set!
	expSet::iterator finder;
	if (mathCore::is_var(a)) {
		if (evaluationSet.end() == (finder = evaluationSet.find(a))) {
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


/*
	A very naieve way of turning into CNF, It doesn't use the actual essential prime implicants!
	Requires DNF to work!

	NOT IMPLEMENTED :(
*/

void mathCore::to_DNF(expression *a) {
	if (mathCore::is_DNF(a)) { // If we are already a DNF just return!
		// Normally you need to be a minimized one, we don't bother doing that!
		return;
	}
	else if (mathCore::is_or(a)) {
		// Easy case
		for (auto child : a->contents) {
			mathCore::to_DNF(child);	// Our children need to be DNF's
		}
		// Our children are all either Ors or Sums of Ors
	}
}

/*
A very naieve way of turning into CNF, It doesn't use the actual essential prime implicants!
Requires DNF to work!
*/

void mathCore::to_CNF(expression *a) {
	if (mathCore::is_CNF(a)) { // If we are already a CNF just return!
							   // Normally you need to be a minimized one, we don't bother doing that!
		return;
	}
	else if (mathCore::is_or(a)) {
		// Easy case
		for (auto child : a->contents) {
			mathCore::to_DNF(child);	// Our children need to be DNF's
		}
		// Our children are all either Ors or Sums of Ors
	}
}