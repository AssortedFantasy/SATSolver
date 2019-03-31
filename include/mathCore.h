#ifndef _MATHCORE_H
#define _MATHCORE_H

#include "expression.h"
#include <vector>
#include <string>
#include <unordered_map>

class mathCore {
public:
	// Different ways to construct expressions

	// Binary
	static expression* literal(const bool val);
	static expression* literal_true();
	static expression* literal_false();
	static expression* variable(const std::string& identifier);
	static expression* binary_and(const expression* a, const expression* b);
	static expression* binary_or(const expression* a, const expression* b);
	static expression* binary_xor(const expression* a, const expression* b);
	static expression* binary_equiv(const expression* a, const expression* b);

	// Large Expressions
	static expression* multi_and(const std::vector<expression*>& children);
	static expression* multi_or(const std::vector<expression*>& children);
	static expression* multi_xor(const std::vector<expression*>& children);
	static expression* multi_equiv(const std::vector<expression*>& children);

	// Implys
	static expression* imply(const expression* a, const expression* b);

	// Interacting with Functions
	static void negate(expression* a);
	static void dual(expression* a);
};



#endif
