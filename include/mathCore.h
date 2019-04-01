#ifndef _MATHCORE_H
#define _MATHCORE_H

#include "expression.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

class mathCore {
public:
	// MATH CORE CONSTRUCT METHODS
	// Binary
	static expression* literal(const bool val);
	static expression* literal_true();
	static expression* literal_false();
	static expression* variable(const std::string& identifier);
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
	static expression* imply(expression* a, expression* b);

	// Interacting with Functions
	static void negate(expression* a);
	static void dual(expression* a);




	// MATH CORE STREAM METHODS
	// As strings!
	static std::string as_string(expression* a);




	// MATH CORE COMPARE METHODS

	// Flag Checking
	static bool is_literal_true(expression* a);
	static bool is_literal_false(expression* a);
	static bool is_literal(expression* a);
	static bool is_var(expression* a);
	static bool is_negated(expression* a);
	static bool is_dualed(expression* a);
	static bool is_and(expression* a);
	static bool is_or(expression* a);
	static bool is_xor(expression* a);
	static bool is_imply(expression* a);
	static bool is_equiv(expression* a);



private:
	// Under the hood we don't copy tons and tons of strings everywhere
	static void string_streamify(std::stringstream& OutStream, expression* a);
};



#endif
