#ifndef _MATHCORE_H
#define _MATHCORE_H

#include "expression.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <set>

void mergeMultiSet(expSet& dest, expSet& src);	// Simple Helper function

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

	// Implys
	static expression* imply(expression* a, expression* b);

	// Change whatever expression something is, into an AND OR XOR EQUIV
	// doesn't do it with laws, it literally just changes flags
	// DANGEROUS TRANSMUTING FUNCTION CALLS!
	static void trans_and(expression* a);
	static void trans_or(expression* a);
	static void trans_xor(expression* a);
	static void trans_equiv(expression* a);
	static void trans_imply(expression* a);
	static void trans_var(expression* a, unsigned int uuid);
	static void trans_true(expression* a);
	static void trans_false(expression* a);
	static void trans_lit(expression* a, bool val);

	/* The above functions invalidate the tree! So you need to fix it*/
	/* Returns true if something actually got changed! */
	/* DEPRECATED - BAD FIX
	static bool invalidUUID(expression* a);
	static void fixUUID(expression* a);
	*/

	// Large Expressions DEPRECATED
	/*
	static expression* multi_and(const std::vector<expression*>& children);
	static expression* multi_or(const std::vector<expression*>& children);
	static expression* multi_xor(const std::vector<expression*>& children);
	static expression* multi_equiv(const std::vector<expression*>& children);
	*/

	// Interacting with Functions
	static void negate(expression* a);
	static void dual(expression* a);
	static void un_dual(expression * a);
	static void un_negate(expression * a);

	// Recursively Copying Expression
	static expression* copy(expression * a);


	// MATH CORE STREAM METHODS
	// As strings!
	static std::string as_string(expression* a);

	// MATH CORE COMPARE METHODS

	// Flag Checking
	static bool is_literal(expression* a);
	static bool is_var(expression* a);
	static bool is_negated(expression* a);
	static bool is_dualed(expression* a);
	static bool is_and(expression* a);
	static bool is_or(expression* a);
	static bool is_xor(expression* a);
	static bool is_imply(expression* a);
	static bool is_equiv(expression* a);
	static bool var_equal(expression * a, expression * b);
	static void delete_children(expression* a); // Delete all children of A

	// Only returns true if an expression is a pure sum or product
	// of variables and or literals!
	static bool is_product(expression* a);
	static bool is_sum(expression *a);
	static bool same_sign(expression * a, expression* b);

	static bool is_CNF(expression* a);
	static bool is_DNF(expression* a);


	// MATH CORE SIMPLIFY METHODS

	// NON RECURSIVE VERSIONS!
	static void combine_and(expression * a);
	static void combine_or(expression * a);
	static void combine_xor(expression * a);
	static void combine_equiv(expression * a);

	// Standardizer
	static void standardize(expression * a);
	static void xor_standardizer(expression* a); // Helpers
	static void equiv_standarizer(expression* a);

	// DeMorgansLaw, DemorgansD is the dual version!
	static void DeMorgans(expression* a);
	static void DeMorgansD(expression* a);

	// Universal Bound Laws
	static void universal_bound(expression* a);						// Transmuting Call

	// Idempotent Laws
	static void idempotent_law(expression* a);						// Transmuting Call

	// If you remove everything from an expression, that expression becomes the thing!
	// WE DON'T HAVE PARANTHEISIS INTERNALLY!
	static void empty_expression(expression* a);					// Transmuting Call

	// RECURSIVE VERSIONS!
	static void recursive_combine_and(expression* a);
	static void recursive_combine_or(expression* a);
	static void recursive_combine_xor(expression* a);
	static void recursive_combine_equiv(expression* a);

	static void recursive_standardize(expression* a);
	static void recursive_idempotent(expression * a);				// Transmuting Call

	// Evaluate a Variable
	static void evaluate(expression * a, expSet& evaluationSet);	// Transmuting Call

	// Distributive Law
	static void distributive_law(expression* a, expression* distTo, expression* dist);	// Transmuting Call, Sorta

	// Absorbtion Law
	static void absorbtion_law(expression* a, expression* pow);	// Transmuting call on pow!

	static void to_CNF(expression * a);		// Transmuting Call
	static void to_DNF(expression * a);		// Transmuting Call by duality

	// Uses these helper functions!
	static void do_expansion(expression* a, expression* expandTo, expression* expandFrom);
	static void do_simplification(expression * a);

private:
	static void clear_type_flag(expression * a);
	static expression* varFromUUID(unsigned int uuid);

	// Under the hood we don't copy tons and tons of strings everywhere
	static void string_streamify(std::stringstream& OutStream, expression* a);

	// Simplifcation Generics
	static void combine_generics(expression * a, bool(*typeFunction)(expression*));
	static expression* construct_global_var();

	static expression* global_literal;
	static expression* global_variable;

};

#endif
