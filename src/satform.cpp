#include "satform.h"

void reduce_associative(expression* a) {
	// Reduce the associative Components of this
	mathCore::combine_and(a);
	mathCore::combine_or(a);
	mathCore::combine_equiv(a);
	mathCore::combine_xor(a);
}