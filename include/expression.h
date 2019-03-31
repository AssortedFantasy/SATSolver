#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include <vector>

// TODO Move to own .h file?
// Flags
#define LIT_VAL (1<<0);
#define NEGATED (1<<1);
#define DUAL	(1<<2);
#define COMPL	(1<<3);


/*
	Expressions are our main class through which everything interacts
*/
class expression{
private:
	int uuid;
	int FLAGS;
	std::vector<expression> contents;
};


#endif
