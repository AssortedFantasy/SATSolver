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
	static expression* literal(const std::string& identifier);
	static expression* binary_and(expression* a, expression* b);
	static expression* binary_or(expression* a, expression* b);
	static expression* binary_xor(expression* a, expression* b);
	static expression* binary_equiv(expression* a, expression* b);

	// Large Expressions
	static expression* multi_and(const std::vector<expression*>& children);
	static expression* multi_or(const std::vector<expression*>& children);
	static expression* multi_xor(const std::vector<expression*>& children);
	static expression* multi_equiv(const std::vector<expression*>& children);

	// Implys
	static expression* equiv(const expression* a, expression* b);


	// Interacting with Functions
	static void negate(expression* a);
	static void dual(expression* b);
};



#endif
