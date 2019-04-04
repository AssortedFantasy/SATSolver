#include "mathCore.h"

void mergeMultiSet(std::multiset<expression*, compareUUID>& dest,
	std::multiset<expression*, compareUUID>& src) {

	auto hint = dest.begin();
	// We use hints, this isn't quite as fancy as the .merge which is implement in c++17
	// which uses the underlying data structure to very quickly perform the merge and
	// doesn't do pointless memory allocations, but its close!
	for (auto child : src) {
		hint = dest.insert(hint, child);
	}
}

/*
	Associative operations can be combined into a larger version, this is a generic combiner
	all others call this
	Not recursive method!
*/
void mathCore::combine_generics(expression * a, bool(*typeFunction)(expression*)) {
	std::multiset<expression*, compareUUID> temp_set; // Temporary Storage for new elements
	// Then if any of our children happen to be and statements
	// we merge them into ourselves, which is a feature from C++17!

	if ((*typeFunction)(a)) {
		auto iter = a->contents.begin();
		while (iter != a->contents.end()) {
			// If the function is negated or dualed, we can't pull them up into ourselves!
			if (!(mathCore::is_negated(a) || mathCore::is_dualed)) {
				if ((*typeFunction)(*iter)) {

					/*
					MSVC Does not support this, thats really sad, so we have to build our own merge function!
					a->contents.merge(child->contents);
					*/
					mergeMultiSet(temp_set, (*iter)->contents);
					(*iter)->contents.clear(); // Clearing first before deleting it to prevent recursive deletion
					delete (*iter);

					// Erase returns the next iter, since deleting things invalidates it
					iter = a->contents.erase(iter);
				}
				else {
					iter++;
					// Slight Optimization: All variable expressions have 0 uuid, so they must be at the beginning!
					if (iter != a->contents.end()) {
						if (mathCore::is_var(*iter) || mathCore::is_literal) {
							iter = a->contents.end();
						}
					}
				}
			}
		}
		// Finally merge everything we just found into the parent
		mergeMultiSet(a->contents, temp_set);
	}
};


/*
	Using the generic, we construct the combiners of all of the functions which are
	associative
	AND, OR, XOR and EQUIV are all associative
*/
void mathCore::combine_and(expression * a) {
	mathCore::combine_generics(a, mathCore::is_and);
};
void mathCore::combine_or(expression * a) {
	mathCore::combine_generics(a, mathCore::is_or);
};
void mathCore::combine_xor(expression * a) {
	mathCore::combine_generics(a, mathCore::is_xor);
};
void mathCore::combine_equiv(expression * a) {
	mathCore::combine_generics(a, mathCore::is_equiv);
};


/* Standardizer: Everything is turned into exclusively NOT OR AND expressions */
/* NO NAND or NOR!*/
void mathCore::standardize(expression* a) {
	// Vars and Literals are Always Simple
	if (mathCore::is_var(a) || mathCore::is_literal(a)) {
		return;
	}
	// ANDS and ORs are treated the same way!
	else if (mathCore::is_and(a) || mathCore::is_or(a)) {
		if (mathCore::is_negated(a)) {
			mathCore::DeMorgans(a);
		}
		if (mathCore::is_dualed(a)) {
			mathCore::DeMorgansD(a);
		}
	}
	// Implications are expressed as there typical form!
	else if (mathCore::is_imply(a)) {
		// a->b is the same as
		// ~a or b
		trans_or(a);
		mathCore::negate(*(a->contents.begin()));
		// We might still be dualed or nergated, so we need to run this again!
		mathCore::standardize(a);
	}
	// XOR is slightly tricky, since it actually expands like mad!
	// Lots and lots of copies need to made, which is really hard!
	// XOR in general is intractable im fairly sure!
	else if (mathCore::is_xor(a)) {
		mathCore::xor_standardizer(a);
		// We might still be dualed or negated so we call ourself again
		mathCore::standardize(a);
	}
	// STILL NEED EQUIVALENCE!
};


// DeMorgans for Negation!
// Only does anything for AND OR statements, everything else is Idempodent
void mathCore::DeMorgans(expression* a) {
	if (mathCore::is_and(a)) {
		negate(a);
		trans_or(a);
		for (auto child : a->contents) {
			negate(child);
		}
	} 
	else if (mathCore::is_or(a)) {
		negate(a);
		trans_and(a);
		for (auto child : a->contents) {
			negate(child);
		}
	}
}


// Dual Demorgans!
void mathCore::DeMorgansD(expression* a) {
	if (mathCore::is_and(a)) {
		dual(a);
		trans_or(a);
		for (auto child : a->contents) {
			dual(child);
		}
	}
	else if (mathCore::is_or(a)) {
		dual(a);
		trans_and(a);
		for (auto child : a->contents) {
			dual(child);
		}
	}
}


// a @ b @ c = (~a*b + ~b*a) @ c
// We do this pairwise till the entire thing is done!
void mathCore::xor_standardizer(expression* a) {
	if (mathCore::is_xor(a)) {
		// Just by the number of variables you can grasp how much more complicated this is!
		std::multiset<expression*, compareUUID>tempStore;
		std::multiset<expression*, compareUUID>::iterator iter, hintOther;
		expression *first, *second, *firstNew, *secondNew, *lastFirstNew, *lastSecondNew;

		// Keep going until there are at most two things in the tree!
		while (a->contents.size() > 2) {
			// Clear the tempStore, Start at the beginning!
			tempStore.clear();
			iter = a->contents.begin();
			hintOther = tempStore.begin();	// Hints to make this crazy slow thing faster!

			// This runs until we are at the end of the expression!
			while (iter != a->contents.end()) {

				first = *iter;
				iter = a->contents.erase(iter); // Remove it from contents, go forward!
				if (iter == a->contents.end()) { // If we have no other pair just insert first into the tempStore and continue!
					tempStore.insert(hintOther, first);
				}
				else {
					second = *iter;		// Pair of first!
					iter = a->contents.erase(iter);	// Keep moving forward!

					// Construct ~a*b + ~b*a !
					firstNew = mathCore::copy(first);
					secondNew = mathCore::copy(second);
					mathCore::negate(first);
					mathCore::negate(second);

					// Hints keep this faster!
					hintOther = tempStore.insert(hintOther,
						// AND OR of AND's!
						mathCore::binary_or(binary_and(first,secondNew),binary_and(firstNew, second)));
				}

			}
			// Now Merge tempStore into a, and continue all over again!
			mergeMultiSet(a->contents, tempStore);
		}
		
		// Now the XOR has exactly 2 elements!
		iter = a->contents.begin();
		first = *iter;
		iter = a->contents.erase(iter);
		second = *iter;
		iter = a->contents.erase(iter);
		// Remove them and construct the new things which go into the XOR!

		firstNew = mathCore::copy(first);
		secondNew = mathCore::copy(second);
		mathCore::negate(first);
		mathCore::negate(second);

		// Finally insert these last two into the XOR!
		a->contents.insert(binary_and(first, secondNew));
		a->contents.insert(binary_and(firstNew, second));

		// Now we are an OR expression!
		mathCore::trans_or(a);
	}
}