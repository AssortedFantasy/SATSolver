#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include <vector>
#include <string>
#include <unordered_map>
#include <set>

// Flags!
#define F_LIT     (1<<0)
#define F_VAR	  (1<<1)
#define F_NEG	  (1<<2)
#define F_DUAL	  (1<<3)
#define F_AND     (1<<4)
#define F_OR	  (1<<5)
#define F_XOR	  (1<<6)
#define F_IMPLY   (1<<7)
#define F_EQUIV	  (1<<8)

class mathCore; // Predeclaration of mathCore friend
class expression;

// This struct compares UUID's, allowing expressions to exist in multisets 
struct compareUUID {
	bool operator()(const expression* const a, const expression* const b) const;
};

// A type of container for expressions!
typedef std::multiset<expression*, compareUUID> expSet;

/*
	Expressions are our main class through which everything interacts.

	It is completely black boxed, as expressions are their own internal data representation.
	Only MathCore gets to do anything here

	Everything is stored in a multiset

	Some Important Invariants:
	Literal True is F_LIT
	Literal False is F_LIT | F_NEG
	Variables or Literals may not be dualed, only negated!
	
	AND OR XOR IMPLY EQUIV are all mutually exclusive flags
*/
class expression{
public:
	~expression();	// You can delete contructors given to you
private:
	expression();	// But you may not construct an expression yourself

	static std::unordered_map<unsigned int, std::string> uuid_to_string;	// For printing
	static std::unordered_map<std::string, unsigned int> string_to_uuid; // For constructing

	static unsigned int global_uuid;

	int FLAGS = 0;
	int uuid = 0;

	expSet contents;
	// vectors are too slow for operations we would like to use
	//std::vector<expression*> contents;

	// Only mathCore can interact with these directly
	// compareUUID is just needed internally
	friend mathCore;
	friend compareUUID;
};

#endif
