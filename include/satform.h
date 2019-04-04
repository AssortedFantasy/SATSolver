#ifndef _SATFORM_H
#define _SATFORM_H

#include "expression.h"
#include "mathCore.h"

void reduce_associative(expression* a);
void standard_form(expression* a);
void standard_form_raw(expression * a);
void dual_standard(expression * a);
void negated_standard(expression* a);

#endif // !_SATFORM_H
