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
*/
void mathCore::combine_generics(expression * a, bool (*typeFunction)(expression*)) {
	std::multiset<expression*, compareUUID> temp_set; // Temporary Storage for new elements

	// First Call combine_ands on our children
	for (auto child : a->contents) {
		mathCore::combine_generics(child, typeFunction);
	}

	// Then if any of our children happen to be and statements
	// we merge them into ourselves, which is a feature from C++17!
	auto iter = a->contents.begin();

	while (iter != a->contents.end()) {
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
			// Slight Optimization: All variable expressions have 0 uuid, so they must be at the beginning!
			if (iter != a->contents.end()) {
				iter++;
				if (((*iter)->uuid) > 0) {
					iter = a->contents.end();
				}
			}
		}
	}

	// Finally merge everything we just found into the parent
	mergeMultiSet(a->contents, temp_set);

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