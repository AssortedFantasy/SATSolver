#include "satform.h"

void reduce_associative(expression* a) {
	// Reduce the associative Components of this
	mathCore::recursive_combine_and(a);
	mathCore::recursive_combine_or(a);
	mathCore::recursive_combine_xor(a);
	mathCore::recursive_combine_equiv(a);
}

void standard_form(expression* a) {
	reduce_associative(a);
	mathCore::recursive_standardize(a);
	// Now you should be able to do this well
	mathCore::combine_and(a);
	mathCore::combine_or(a);
}

