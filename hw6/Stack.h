#ifndef STACK_H
#define STACK_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */

#include <iostream>
#include "List.h"
using namespace std;

class Stack : private List {
public:
	List :: IsEmpty;
	List :: NumElements;
	List :: Write;

	Base * Pop (void);
	Base * Push (Base *);
	Base * Top (void);
};

#endif
