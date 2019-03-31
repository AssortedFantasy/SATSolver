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

class mathCore; // Predeclaration of mathCore friend

/*
	Expressions are our main class through which everything interacts.

	It is completely black boxed, as expressions are there own internal data representation.
	Only MathCore gets to do anything here
*/
class expression{
public:
	~expression();	// You can delete contructors given to you
private:
	expression();	// But you may not construct an expression yourself

	static std::unordered_map<int, std::string>& uuid_to_string;	// This is used to turn things inside out!
	static int global_uuid;

	int uuid;
	char FLAGS;
	std::vector<expression*> contents;

	// Only mathCore can interact with these directly
	friend mathCore;
};
#endif
