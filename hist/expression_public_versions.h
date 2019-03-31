#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include <vector>
#include <string>
#include <unordered_map>

// Flags!
#define F_LIT_VAL (1<<0)
#define F_NEG	  (1<<1)
#define F_DUAL	  (1<<2)
#define F_AND     (1<<3)
#define F_OR	  (1<<4)
#define F_XOR	  (1<<5)
#define F_IMPLY   (1<<6)
#define F_EQUIV	  (1<<7)

/*
	Expressions are our main class through which everything interacts
	Pointers are going to be flying everywhere, so be very careful with this!

	- They need to be easy to construct, as new expressions for interperting
	- They need to have output methods, to return as strings

*/
class expression{
public:
	expression(const expression& other);	// Copy Constructor, So you don't Segfault if you copy accidentally.
	~expression();							// Recursive destructor, make sure there are no cycles or duplicates
	std::string asString() const;			// Transforms it into a string
	std::string asStringDense() const;		// Dense Strings with no spaces
private:
	static std::unordered_map<int, std::string>& uuid_to_string;	// This is used to turn things inside out!
	int uuid;
	int FLAGS;
	std::vector<expression*> contents;

	// Only these functions which are safe get to actually do this magic!
	friend expression* lit_expression(const std::string& idenfier);
	friend expression* binary_and_expression(expression* a, expression* b);
};

expression* lit_expression(const std::string& idenfier);
expression* binary_and_expression(expression* a, expression* b);
	

#endif
