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
	A very naieve way of turning into DNF, It doesn't use the actual essential prime implicants!
	Requires CNF to work!
*/
void mathCore::to_DNF(expression *a) {
	if (mathCore::is_DNF(a)) { // If we are already a DNF just return!
		// Normally you need to be a minimized one, we don't bother doing that!
		recursive_idempotent(a);
		return;
	}
	else if (mathCore::is_or(a) && !(is_dualed(a) || is_negated(a))) {
		// If we are an OR statement then fancy things can happen

		expSet tempSet;
		expression* child;
		auto iter = a->contents.begin();
		while (iter != a->contents.end()) {
			child = *iter;
			iter = a->contents.erase(iter);

			to_DNF(child);
			tempSet.insert(child);
		}
		mergeMultiSet(a->contents, tempSet);
		// Our children now are DNFED!

		// Pull our children up together
		combine_or(a);
		// The only simplifcation we do is idempotent
		mathCore::idempotent_law(a);
		mathCore::universal_bound(a);

		/*
		Need Quine–McCluskey algorithm or Similar here
		Currently its dumb!
		*/
	}
	else if (mathCore::is_and(a) && !(is_dualed(a) || is_negated(a))) {
		// If we are an and statement
		expSet tempSet;
		expression* child;
		auto iter = a->contents.begin();
		while (iter != a->contents.end()) {
			child = *iter;
			iter = a->contents.erase(iter);

			to_CNF(child);
			tempSet.insert(child);
		}
		mergeMultiSet(a->contents, tempSet);
		// Our children now are CNFED!
		// First pull them all up into ourselves
		recursive_combine_and(a);
		mathCore::idempotent_law(a);
		mathCore::universal_bound(a);

		// If we became a valid DNF return
		if (is_DNF(a)) {
			return;
		}

		// We aren't a valid DNF, so something needs to be expanded
		// Iter is definitinitly a sum
		iter = a->contents.begin();
		expression* big_term = *iter;
		iter = a->contents.erase(iter);

		while (iter != a->contents.end()) {
			child = *iter;
			iter = a->contents.erase(iter);
			do_expansion(a, big_term, child);
		}

		// Put only big term back!
		a->contents.insert(big_term);
		empty_expression(a);	// Transmuting call!
	}
}


/*
A very naieve way of turning into CNF, It doesn't use the actual essential prime implicants!
Requires DNF to work!
*/
void mathCore::to_CNF(expression *a) {
	if (mathCore::is_CNF(a)) { // If we are already a CNF just return!
							   // Normally you need to be a minimized one, we don't bother doing that!
		recursive_idempotent(a);
		return;
	}
	else if (mathCore::is_and(a) && !(is_dualed(a) || is_negated(a))) {
		// If we are an AND statement then fancy things can happen

		expSet tempSet;
		expression* child;
		auto iter = a->contents.begin();
		while (iter != a->contents.end()) {
			child = *iter;
			iter = a->contents.erase(iter);

			to_CNF(child);
			tempSet.insert(child);
		}
		mergeMultiSet(a->contents, tempSet);
		// Our children now are CNFED!

		// Pull our children up together
		combine_and(a);
		// The only simplifcation we do is idempotent
		mathCore::idempotent_law(a);
		mathCore::universal_bound(a);

		/*
		Need Quine–McCluskey algorithm or Similar here
		Currently its dumb!

		OR you can use Tseytin transformation for polynomial time
		*/
	}
	else if (mathCore::is_or(a) && !(is_dualed(a) || is_negated(a))) {
		// If we are an and statement
		expSet tempSet;
		expression* child;
		auto iter = a->contents.begin();
		while (iter != a->contents.end()) {
			child = *iter;
			iter = a->contents.erase(iter);

			to_DNF(child);
			tempSet.insert(child);
		}
		mergeMultiSet(a->contents, tempSet);
		// Our children now are CNFED!
		// First pull them all up into ourselves
		recursive_combine_and(a);
		mathCore::idempotent_law(a);
		mathCore::universal_bound(a);

		// If we became a valid DNF return
		if (is_CNF(a)) {
			return;
		}

		// We aren't a valid CNF, so something needs to be expanded
		// Iter is definitinitly a product
		iter = a->contents.begin();
		expression* big_term = *iter;
		iter = a->contents.erase(iter);

		while (iter != a->contents.end()) {
			child = *iter;
			iter = a->contents.erase(iter);
			do_expansion(a, big_term, child);
		}

		// Put only big term back!
		a->contents.insert(big_term);
		empty_expression(a);	// Transmuting call!
	}
}


void mathCore::do_expansion(expression* a, expression* expandTo, expression* expandFrom) {
	if (is_and(a)) {
		// (ABC+DEF)(X+Y)
		// (ABC(X+Y) + DEF(X+Y))
		if (is_DNF(expandTo) && is_sum(expandFrom)) {
			distributive_law(a, expandTo, expandFrom);
			
			expSet tempSet;
			auto iter = expandTo->contents.begin();
			expression* child;

			while (iter != expandTo->contents.end()) {
				child = *iter;
				iter = expandTo->contents.erase(iter);
				do_simplification(child);
				tempSet.insert(child);
			}
			mergeMultiSet(expandTo->contents, tempSet);
			idempotent_law(a);
			universal_bound(a);
		}
	}
	else if (is_or(a)) {
		// (A+B+C)(D+E+F) + (ABCD)
		// (A+B+C+ABCD)(D+E+F+ABCD)
		if (is_CNF(expandTo) && is_product(expandFrom)) {
			distributive_law(a, expandTo, expandFrom);

			expSet tempSet;
			auto iter = expandTo->contents.begin();
			expression* child;

			while (iter != expandTo->contents.end()) {
				child = *iter;
				iter = expandTo->contents.erase(iter);
				do_simplification(child);
				tempSet.insert(child);
			}
			mergeMultiSet(expandTo->contents, tempSet);
			idempotent_law(a);
			universal_bound(a);
		}
	}
}


void mathCore::do_simplification(expression* a) {
	if (is_and(a)) {
		combine_and(a);

		auto iter = a->contents.begin();
		expression* child = *iter;
		iter = a->contents.erase(iter);	// Pop it!

		if (is_or(child)) {
			absorbtion_law(a, child);

			if (is_literal(child) || is_var(child)) {
				a->contents.insert(child); // Insert it back if its simple
			}
			else {
				// a is our original parent and where the answer needs to go, so we can't distribute it
				// we distribute a copy!
				distributive_law(a, child, mathCore::copy(a));
				
				delete_children(a);
				a->contents.clear();
				a->contents.insert(child);
			}
			empty_expression(a);
		}
	}
	else if (is_or(a)) {
		combine_or(a);

		auto iter = a->contents.begin();
		expression* child = *iter;
		iter = a->contents.erase(iter);	// Pop it!

		if (is_and(child)) {
			absorbtion_law(a, child);

			if (is_literal(child) || is_var(child)) {
				a->contents.insert(child); // Insert it back if its simple
			}
			else {
				// a is our original parent and where the answer needs to go, so we can't distribute it
				// we distribute a copy!
				distributive_law(a, child, mathCore::copy(a));

				delete_children(a);
				a->contents.clear();
				a->contents.insert(child);
			}
			empty_expression(a);
		}
	}
}
