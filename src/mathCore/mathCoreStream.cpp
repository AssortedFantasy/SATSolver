#include "mathCore.h"

std::string mathCore::as_string(expression* a) {
	std::stringstream OutStream;
	mathCore::string_streamify(OutStream, a);
	return OutStream.str();
}

void mathCore::string_streamify(std::stringstream& OutStream, expression* a) {
	if (mathCore::is_literal(a)) {
		if (mathCore::is_negated(a)) {
			OutStream << " 0 ";
		}
		else {
			OutStream << " 1 ";
		}
	}
	else if (mathCore::is_var(a)) {
		if (mathCore::is_negated(a)) {
			OutStream << " ~" << expression::uuid_to_string.at(a->uuid) << " "; // Note the little quote
		}
		else {
			OutStream << " " << expression::uuid_to_string.at(a->uuid) << " ";
		}
	}
}