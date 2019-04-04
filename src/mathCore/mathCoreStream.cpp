#include "mathCore.h"

std::string mathCore::as_string(expression* a) {
	std::stringstream OutStream;
	mathCore::string_streamify(OutStream, a);
	return OutStream.str();
}

void mathCore::string_streamify(std::stringstream& OutStream, expression* a) {
	if (mathCore::is_literal(a)) {
		if (mathCore::is_negated(a)) { // Handling Literals
			OutStream << " 0 ";
		}
		else {
			OutStream << " 1 ";
		}
	}
	else if (mathCore::is_var(a)) {	// Handling Vars
		if (mathCore::is_negated(a)) {
			OutStream << " ~" << expression::uuid_to_string.at(a->uuid) << " "; // Note the tilde
		}
		else {
			OutStream << " " << expression::uuid_to_string.at(a->uuid) << " ";
		}
	}
	else {	// Everything else, aka AND OR XOR IMPLY EQUIV, dualed or not dualed
		// Handle Tilde at beginning
		if (mathCore::is_negated(a)) {
			OutStream << " ~(";
		}
		else {
			OutStream << " (";
		}

		if (!(a->contents.size())) {
			// BAD Things are happening
			throw;
		}
		std::string operand_symbol = "!MISSING!";

		// AND OR XOR IMPLY EQUIV all need to recursively call down!
		if (mathCore::is_and(a)) {
			operand_symbol = "*";
		}
		else if (mathCore::is_or(a)) {
			operand_symbol = "+";
		}
		else if (mathCore::is_xor(a)) {
			operand_symbol = "@";
		}
		else if (mathCore::is_imply(a)) {
			operand_symbol = "->";
		}
		else if (mathCore::is_equiv(a)) {
			operand_symbol = "=";
		}

		// Now actually do the streamifying
		auto it = a->contents.begin();
		if (it != a->contents.end()) {
			mathCore::string_streamify(OutStream, *it); // Streamify the first element

			for (it++; it != a->contents.end(); it++) {
				OutStream << operand_symbol;	       // Streamify every other element
				mathCore::string_streamify(OutStream, *it);
			}
		}

		// Handle Parenthesis at end
		if (mathCore::is_dualed(a)) {
			OutStream << ")^d ";
		}
		else {
			OutStream << ") ";
		}
	}
}