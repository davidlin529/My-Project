/******************************************************************************

							John David Lin
							CSE 12, Spring 2017
							April 25, 2017
							cs12xkz
				Assignment Four
File Name:	stack.c
Description:	This program program simulates a top-of-the-line calculator 
		that will perform such advanced commands as addition, 
		subtractions, multiplication, division, exponentiation, and 
		factorial! The program will accept mathematical expressions and
		perform operations.

*******************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define SIGN_BIT (1L << ((sizeof (long) << 3) - 1))
#define BYTE 8
#define FACT_INDEX 8 /* the index of factorial in operators */

static char operators[] = "()+-*/^ !";


static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };

/*--------------------------------------------------------------------------
Function Name:          eval
Purpose:                Evaluates the result of the mathematical expressions 
			from "postfix" notation.
Description:            While stack2 is not empty, pop numbers from stack2, 
			pushing all digits popped to stack1.  Once a 
			non-numbers is encountered from stack2, the 
			appropriate number of operands will be popped from 
			stack1 and evaluated using the operator just popped 
			from stack2.  The result of this computation will be
			pushed on stack1.
Input:                  stack1: the pointer to the main stack
Result:                 The postfix expression is evaluated
			Result: the value of the expression
--------------------------------------------------------------------------*/
long eval (Stack * stack1) {
/* [remove after use]
ALGORITHM FOR POST-FIX EVALUATION:
You will need 2 stacks for this algorithm.  Your function "eval" will
take a pointer to a stack (stack1) as a parameter.  It will use this stack
as one of its needed stacks.  The other stack will be local.  After the
evaluation, the parameter stack will be empty.

Reverse stack1 onto stack2, then begin the evaluation:
While stack2 is not empty
     pop numbers from stack2, pushing all digits popped to stack1.  Once a
     non-numbers is encountered from stack2, the appropriate number of
     operands will be popped from stack1 and evaluated using the operator
     just popped from stack2.  The result of this computation will be
     pushed on stack1.
The final result that remains on stack1 is the final result of the
expression.  This result is then used as the return value for this
function.  During the evaluation process, stack2 holds positive integers
and operators, but stack1 only holds signed integers.
*/

	long op1, op2, word; /* for the operands and the operator */
	
	Stack * stack2 = new_Stack(CALCSTACKSIZE); /* stack 2 initialized */
	long character; /* to hold value from stack for reversing */
	long result;	/* the value of the expression */

	/* Reversing stack1 onto stack2 */
	while (!isempty_Stack(stack1)) {
		pop(stack1, &character);
		push(stack2, character);
	}
	
	/* Evaluate the expression */
	while (!isempty_Stack(stack2)) {
		top(stack2, &character);

		/* Push numbers from stack1 to stack2 */
		if (character >= 0) {
			pop(stack2, &character);
			push(stack1, character);
		}

		/* Operation with operators */
		else {
			/* Pop only 1 number for factorial */
			if (INDEX(character) == FACT_INDEX) {
				pop(stack1, &op1);
			}

			/* Pop 2 numbers for other operations */
			else {
				pop(stack1, &op1);
				pop(stack1, &op2);
			}
			pop(stack2, &word);

			/* Evaluate the expression with the corresponding
			 * operation
			 */
			result = functions[INDEX(word)](op1, op2);
			push(stack1, result); /* Save the result */
		}

	}
	pop(stack1, &result);	/* Save the result in stack1 */
	delete_Stack(&stack2);  /* Deallocate stack2 */
	return result;		/* retrn the result */
}

/*--------------------------------------------------------------------------
Function Name:          intopost
Purpose:                Convert "infix" mathematical expressions entered 
			by the user into their "postfix" equivalents.
Description:            If character is a digit, then continue to read digits
			until it reads a non-digit, converting this number to
			decimal. Store this decimal number on stack1. For 
			operators, repeatedly push to stack1 what is popped 
			from stack2 until stack2 is empty or stack2's top 
			symbol has a lower priority than the character entered.
			Then push the character onto stack2.
Input:                  stack1: the pointer to the main stack
Result:                 The "infix" mathematical expressions entered 
			is converted. 
			Result: the pointer to the main stack.
--------------------------------------------------------------------------*/
long intopost (Stack * stack1) {
/* [remove after use]
ALGORITHM FOR INFIX-TO-POSTFIX:
You will need 2 stacks for this algorithm.  Your function "intopost" will
take a pointer to a stack (stack1) as a parameter.  It will use this stack
as one of its needed stacks.  The other stack will be local.  The
parameter stack will also serve as the place to store the resultant
post-fix expression.  The return value from intopost is either EOF
when ^D is entered or a non-zero value indicating that the function
succeeded.

Process each character of the input in turn
     if character is EOF, return EOF
     if character is blank, then ignore it
     if character is a digit, then continue to read digits until you read
          a non-digit, converting this number to decimal as you go.  Store
          this decimal number on stack1
     else if character is '(' then push it to stack2
     else if character is ')' then repeatedly pop stack2, pushing all
          symbols popped from stack2 onto stack1 until the first '('
          encountered is popped from stack2.  Discard '(' and ')'
     else repeatedly push to stack1 what is popped from stack2
          until stack2 is empty or stack2's top symbol has a lower
          priority than the character entered.  Then push the character
          onto stack2.
After processing all characters, pop anything remaining on stack2, pushing
all symbols popped from stack2 to stack1.  Stack1 now contains the
post-fix expression, in reverse order.
*/

	Stack * stack2 = new_Stack(CALCSTACKSIZE); /* stack 2 initialized */
	long character;	/* to hold value from stack */
	long number;	/* to hold numbers */

	/* convert "infix" mathematical expressions entered by the user 
	 * into their "postfix" equivalents.
	 */
	while (1) {
		character = fgetc(stdin); /* gets input from the buffer */

		/* at the end of the line, pop the remaining on stack2
		 * onto stack1 
		 */
		if (character == '\n') {

			/* while stack2 is not empty, pop from 
			 * stack2 to stack1 */
			while (!isempty_Stack(stack2)) {
				pop(stack2, &character);
				push(stack1, character);
			}
			delete_Stack(&stack2); /* deallocate stack2 */
			break;		/* exit the loop */
		}
		
		/* return EOF */
		else if (character == EOF) {
			delete_Stack(&stack2); /* deallocate stack2 */
			return EOF;
		}

		/* ignore a whitespace */
		else if (character == ' ') {
		}

		/* if the character is a number, call decin and 
		 * store the number in stack 1
		 */
		else if ((character <= '9') && (character >= '0')) {
			
			/* throw the number back to the buffer for 
			 * decin to process 
			 */
			ungetc(character, stdin);
			number = decin();
			push(stack1, number); /* push to stack1 */
		}

		/* push '(' to stack2 */
		else if (character == '(') {
			character = setupword(character);
			push(stack2, character); /* push to stack2 */
		}

		/* pop stack2 onto stack1 until a '(' */
		else if (character == ')') {
			pop(stack2, &character); 
			/* keep popping from stack2 to stack1
			 * until '(' */
			while (CHARACTER(character) != '(') {
				push(stack1, character);
				pop(stack2, &character);
			}
		}
		
		/* set up properties for operators and push them to 
		 * stack2
		 */
		else {	
			long top_char;	/* holds the top of stack2 */

			/* repeatedly push to stack1 what is popped from 
			 * stack2 until stack2 is empty or stack2's top 
			 * symbol has a lower priority than the character 
			 * entered. Then push the character onto stack2.
			 */
			while (!isempty_Stack(stack2)) {
				top(stack2, &top_char);
				
				/* checks priority */
				if (PRIORITY(top_char) >= 
					PRIORITY(setupword(character))) {
					/* pop from stack2 to stack1 */
					pop(stack2, &top_char);
					push(stack1, top_char);
				}

				/* exit the loop */
				else
					break;
			}
			
			/* set up the operators and push them to stack2 */
			character = setupword(character); 
			push(stack2, character);	
		}
	}
	return * stack1;	/* return the pointer to stack1 */
}

static long add (long augend, long addend) {
    return augend + addend;
}

static long divide (long divisor, long dividend) {
    return dividend / divisor;
}

static long exponent (long power, long base) {
	long result;	/* holds the result */
	result = base;	
	power --;	/* decrement the power */

	/* keeps multiplying until power is 0 */
	while (power != 0) {
		result *= base; /* multiplication */
		power --;	/* decrement the power */
	}
	return result;	/* return the result */
}

static long fact (long xxx, long ignored) {
	long result;	/* holds the result */
	result = xxx;
	xxx --;		/* decrement the number */

	/* keeps multiplying until 0*/
	while (xxx != 0) {
		result *= xxx;	/* multiplication */
		xxx--;		/* decrement the number */
	}
	return result;	/* return the result */
}

static long mult (long factorx, long factory) {
    return factorx * factory;
}

static long setupword (int character) {
	long index = 0;	/* used for operators array */

	/* search for the operator in operators array */
	while (operators[index]) {
		/* check if operator is found */
		if (character == operators[index]) {
			break;
		}
		index ++;	/* increment index */
	}
	/* create the operator for the stack */
	return (SIGN_BIT | (index << BYTE) | character);
}

static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

