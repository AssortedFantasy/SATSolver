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

		if (!a->contents.size()) {
			// BAD Things are happening
			throw;
		}

		// AND OR XOR IMPLY EQUIV all need to recursively call down!
		if (mathCore::is_and(a)) {

			for (int i = 0; i + 2 < a->contents.size(); i++) { // Iterate through except the last
				mathCore::string_streamify(OutStream, a->contents.at(i));
				OutStream << "*";
			}
			mathCore::string_streamify(OutStream, a->contents.back());
		
		}
		else if (mathCore::is_or(a)) {

			for (int i = 0; i + 2 < a->contents.size(); i++) { // Iterate through except the last
				mathCore::string_streamify(OutStream, a->contents.at(i));
				OutStream << "+";
			}
			mathCore::string_streamify(OutStream, a->contents.back());

		}
		else if (mathCore::is_xor(a)) {

			for (int i = 0; i + 2 < a->contents.size(); i++) { // Iterate through except the last
				mathCore::string_streamify(OutStream, a->contents.at(i));
				OutStream << "@";
			}
			mathCore::string_streamify(OutStream, a->contents.back());

		}
		else if (mathCore::is_imply(a)) {
			// MUST HAVE TWO!
			mathCore::string_streamify(OutStream, a->contents.front());
			OutStream << "->";
			mathCore::string_streamify(OutStream, a->contents.back());
		}
		else if (mathCore::is_equiv(a)) {
			for (int i = 0; i + 2 < a->contents.size(); i++) { // Iterate through except the last
				mathCore::string_streamify(OutStream, a->contents.at(i));
				OutStream << "=";
			}
			mathCore::string_streamify(OutStream, a->contents.back());
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