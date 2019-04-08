#include "mathCore.h"

void mergeMultiSet(expSet& dest, expSet& src) {

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
	expSet temp_set; // Temporary Storage for new elements


													  // If our children are the same typefunction as ourselves
													  // we merge them into ourselves, which is a feature from C++17!

	if ((*typeFunction)(a)) {
		auto iter = a->contents.begin();
		while (iter != a->contents.end()) {
			// They need to be the same type as us and also not be negated or dualed!
			if ((*typeFunction)(*iter) && !(mathCore::is_negated(*iter) || mathCore::is_dualed(*iter))) {
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
					if (mathCore::is_var(*iter) || mathCore::is_literal(*iter)) {
						iter = a->contents.end();
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
	else if (mathCore::is_equiv(a)) {
		mathCore::equiv_standarizer(a);
		mathCore::standardize(a);
	}
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


void mathCore::universal_bound(expression* a) {
	auto literal_pos = a->contents.lower_bound(global_literal);		// Lower bound for literals in the expression
	auto literal_bound = a->contents.upper_bound(global_literal); // Upper bound for literals in the expression

	// If the expression is an AND
	if (mathCore::is_and(a)) {
		// Iterate over all literals
		while (literal_pos != literal_bound) {
			// If a literal 0 is found, the entire expression evaluates to 0
			if (mathCore::is_negated(*literal_pos)) {
				// Delete all the the children of the expression 
				mathCore::delete_children(a);
				// Turn the expression into a false literal
				mathCore::trans_lit(a, false);
				break;
			} else {
				// If the literal found was true, it can be removed
					delete *literal_pos;	// Delete the expression
					literal_pos = a->contents.erase(literal_pos);	// Advance the iterator safely
			}
		}
		// If the expression is an or statement 
	} else if (mathCore::is_or(a)) {
		// Iterate over all literals
		while (literal_pos != literal_bound) {
			// If a literal 1 is found, the entire expression evaluates to 1
			if (!(mathCore::is_negated(*literal_pos))) {
				// Delete all the the children of the expression 
				mathCore::delete_children(a);
				// Turn the expression into a true literal
				mathCore::trans_lit(a, true);
				break;
			} else {
				// If the literal found was false, it can be removed
				delete *literal_pos;	// Delete the expression
				literal_pos = a->contents.erase(literal_pos);	// Advance the iterator safely
			}
		}
	}
	mathCore::empty_expression(a);
}


// a @ b @ c = (~a*b + ~b*a) @ c
// We do this pairwise till the entire thing is done!
void mathCore::xor_standardizer(expression* a) {
	if (mathCore::is_xor(a)) {
		// Just by the number of variables you can grasp how much more complicated this is!
		expSet tempStore;
		expSet::iterator iter, hintOther;
		expression *first, *second, *firstNew, *secondNew;

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
						mathCore::binary_or(binary_and(first, secondNew), binary_and(firstNew, second)));
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


// This is the exact dual of the function above!
void mathCore::equiv_standarizer(expression* a) {
	if (mathCore::is_equiv(a)) {
		// Just by the number of variables you can grasp how much more complicated this is!
		expSet tempStore;
		expSet::iterator iter, hintOther;
		expression *first, *second, *firstNew, *secondNew;

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

													// Construct ~a*~b + a*b !
					firstNew = mathCore::copy(first);
					secondNew = mathCore::copy(second);
					mathCore::negate(first);
					mathCore::negate(second);

					// Hints keep this faster!
					hintOther = tempStore.insert(hintOther,
						// AND OR of AND's!
						mathCore::binary_or(binary_and(first, second), binary_and(firstNew, secondNew)));
				}

			}
			// Now Merge tempStore into a, and continue all over again!
			mergeMultiSet(a->contents, tempStore);
		}

		// Now the EQUIV has exactly 2 elements!
		iter = a->contents.begin();
		first = *iter;
		iter = a->contents.erase(iter);
		second = *iter;
		iter = a->contents.erase(iter);
		// Remove them and construct the new things which go into the EQUIV!

		firstNew = mathCore::copy(first);
		secondNew = mathCore::copy(second);
		mathCore::negate(first);
		mathCore::negate(second);

		// Finally insert these last two into the EQUIV!
		a->contents.insert(binary_and(first, second));
		a->contents.insert(binary_and(firstNew, secondNew));

		// Now we are an OR expression!
		mathCore::trans_or(a);
	}
}


// The idempodent_laws are
// A * A = A
// A * ~A = 0
// A + A = A
// A + ~A = 1
// This takes linear time and applys them!
void mathCore::idempotent_law(expression* a) {
	unsigned int previous_uuid = 0;
	bool found_true = 0;
	bool found_false = 0;

	if (mathCore::is_and(a)) {
		// AND things

		expSet::iterator iter = a->contents.lower_bound(mathCore::global_variable);

		// LowerBound forces this to at be the lowest valid!
		while (iter != a->contents.end()) {
			if ((*iter)->uuid == previous_uuid) {
				// We have a new match
				if (mathCore::is_negated(*iter)) {
					if (found_false) {
						// We already have a false
						delete *iter;
						iter = a->contents.erase(iter);
					}
					else {
						iter++;
						found_false = true;
					}
				}
				// New true match!
				else {
					if (found_true) {
						// We already have a true!
						delete *iter;
						iter = a->contents.erase(iter);
					}
					else {
						iter++;
						found_true = true;
					}
				}

				// Now we have the fancy case!
				if (found_false && found_true) {
					// Fancy case
					mathCore::trans_false(a);
					mathCore::delete_children(a);
					iter = a->contents.end();
				}
			}
			else {
				previous_uuid = (*iter)->uuid;
				found_true = found_false = false;
				found_true = found_false = false;
				if (is_negated(a)) {
					found_false = true;
				}
				else {
					found_true = true;
				}
				iter++;
			}
		}
	}
	else if (mathCore::is_or(a)) {
		// OR things
		expSet::iterator iter = a->contents.lower_bound(mathCore::global_variable);

		// LowerBound forces this to at be the lowest valid!
		while (iter != a->contents.end()) {
			if ((*iter)->uuid == previous_uuid) {
				// We have a new match
				if (mathCore::is_negated(*iter)) {
					if (found_false) {
						// We already have a false
						delete *iter;
						iter = a->contents.erase(iter);
					}
					else {
						iter++;
						found_false = true;
					}
				}
				// New true match!
				else {
					if (found_true) {
						// We already have a true!
						delete *iter;
						iter = a->contents.erase(iter);
					}
					else {
						iter++;
						found_true = true;
					}
				}

				// Now we have the fancy case!
				if (found_false && found_true) {
					// Fancy case
					mathCore::trans_true(a);
					mathCore::delete_children(a);
					iter = a->contents.end();
				}
			}
			else {
				previous_uuid = (*iter)->uuid;
				found_true = found_false = false;
				if (is_negated(a)) {
					found_false = true;
				}
				else {
					found_true = true;
				}
				iter++;
			}
		}
	}
	mathCore::empty_expression(a);
}


// This is distiributitive law!
// You need to pass the bounding expression *a and two iterators, disTo and dist
// the second one is distributed over the first!
// If you pass something that doesn't have a notion of distribution then it just passes!
void mathCore::distributive_law(expression* a, expression* distTo, expression* dist) {
	if (is_and(a)) {
		if (is_or(distTo) && !(is_negated(distTo) || is_dualed(distTo))) {	// It needs to be a clean OR statement to distribute!
			
			expSet tempStore;	// Need temporary storage to not invalidate iterators

			auto hint = tempStore.begin();
			auto iter = distTo->contents.begin();

			// For the first element just use dist!
			hint = tempStore.insert(hint, binary_and(*iter, dist));
			iter = distTo->contents.erase(iter);	// Advances iter

			while (iter != distTo->contents.end()) {
				// Until the end of the iterator keep doing this
				hint = tempStore.insert(hint, binary_and(*iter, mathCore::copy(dist))); // We match these copies with the remaining elements
				iter = distTo->contents.erase(iter);
			}

			// Then at the end just merge tempStore into distTo!
			mergeMultiSet(distTo->contents, tempStore);
		}
	}
	else if (is_or(a)) {
		if (is_and(distTo) && !(is_negated(distTo) || is_dualed(distTo))) {	// It needs to be a clean AND statement to distribute!

			expSet tempStore;	// Need temporary storage to not invalidate iterators

			auto hint = tempStore.begin();
			auto iter = distTo->contents.begin();

			// For the first element just use dist!
			hint = tempStore.insert(hint, binary_or(*iter, dist));
			iter = distTo->contents.erase(iter);	// Advances iter

			while (iter != distTo->contents.end()) {
				// Until the end of the iterator keep doing this
				hint = tempStore.insert(hint, binary_or(*iter, mathCore::copy(dist))); // We match these copies with the remaining elements
				iter = distTo->contents.erase(iter);
			}

			// Then at the end just merge tempStore into distTo!
			mergeMultiSet(distTo->contents, tempStore);
		}
	}
}


/*
	You are given the parent pointer and an
	iterator to a term inside.

	A+Z+(A*X) = A+Z (Pows the whole thing if same sign)!
	A+Z+(~A*X) = A+Z+X (Pows the ~A if opposite sign)

	A*(A+X) = A (Pows the whole and statement!)
	A*(~A+X) = A*X (Pows the ~A if opposite sign)!
*/
void mathCore::absorbtion_law(expression* a, expression* pow){
	if (is_and(a)) {
		if (is_or(pow) && !(is_negated(pow) || is_dualed(pow))) { // Needs to be a clean OR statement to POW things out of it!
			auto iter = pow->contents.lower_bound(global_variable);	// You can only pow variables
			expSet::iterator outsiders;

			while (iter != pow->contents.end()) {
				if (is_var(*iter)) { // Its guarenteed to be a variable anyway
					outsiders = a->contents.find(*iter);
					if (outsiders == a->contents.end()) {	// If we find nothing, go to the next variable
						iter++;
					}
					while(outsiders != a->contents.end()) {
						// We found something outside!
						if (same_sign(*outsiders, *iter)) {
							// Same sign, Pow just became a literal 1!
							delete_children(pow);
							trans_true(pow);
							iter = pow->contents.end();
							break;
						}
						else {
							// Different Signs -> Its irrelevant, so pow loses it!
							iter = pow->contents.erase(iter);
							break;
						}
						outsiders++;
					}
				}
			}
		}
	}
	else if (is_or(a)) {
		if (is_and(pow) && !(is_negated(pow) || is_dualed(pow))) { // Needs to be a clean AND statement to POW things out of it!
			auto iter = pow->contents.lower_bound(global_variable);	// You can only pow variables
			expSet::iterator outsiders;

			while (iter != pow->contents.end()) {
				if (is_var(*iter)) { // Its guarenteed to be a variable anyway
					outsiders = a->contents.find(*iter);
					if (outsiders == a->contents.end()) {	// If we find nothing, go to the next variable
						iter++;
					}
					while (outsiders != a->contents.end()) {
						// We found something outside!
						if (same_sign(*outsiders, *iter)) {
							// Same sign, Pow just became a literal 0!
							delete_children(pow);
							trans_false(pow);
							iter = pow->contents.end();
							break;
						}
						else {
							// Different Signs -> Its irrelevant, so pow loses it!
							iter = pow->contents.erase(iter);
							break;
						}
						outsiders++;
					}
				}
			}
		}
	}
	else if (is_var(a)) {

	}
	// You could theoretically empty pow!
	empty_expression(pow);
}