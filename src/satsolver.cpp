#include <iostream>
#include "expression.h"


/*
	Command Line Interface implementation for the project
	uses getopt() from 
*/
int main(int argc, char* argv[]){
	std::cout << sizeof(long) << "\n" << sizeof(int) << "\n" << sizeof(long long) << "\n" << sizeof(short) << "\n" << sizeof(expression) << "\n";
	std::cin.get();
	return 0;
}
