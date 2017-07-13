/******************************************************************************

							John David Lin
							CSE 12, Spring 2017
							April 19, 2017
							cs12xkz
				Assignment Three
File Name:	stack.c
Description:	This program receives function calls from dirver.c and return
		and/or print the corresponding values about the stack. 

*******************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";	
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";	
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] =						
                        "Isempty check from a non-existent stack!!!\n";		
static const char ISFULL_NONEXIST[] =						
                        "Isfull check from a non-existent stack!!!\n";		
static const char NUM_NONEXIST[] =						
                        "get_occupancy check from a non-existent stack!!!\n";	
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";	
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n";		
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";	
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";			
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";	
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";		
static const char WRITE_NONEXIST_FILE[] =				
                        "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] =					
                        "Attempt to write to a non-existent stack!!!\n";	

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";		
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";		
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";			
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";			
static const char TOP[] = "[Stack %ld - Topping %ld]\n";			

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
        debug = FALSE;
}

void debug_on (void) {
        debug = TRUE;
}

/* start of true stack code */

/*--------------------------------------------------------------------------
Function Name:          delete_Stack
Purpose:                Deallocates the stack
Description:            If the pointer to pointer is valid, call free() to 
			deallocate the stack. And then set the pointer to NULL 
			and decrement the stack_counter. 
Input:                  spp: the pointer to pointer to the stack
Result:                 The stack is deallocated. 
			No return value.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {

	/* checks the pointer to the pointer to the stack for validity */
	if (spp != NULL) {

		/* checks the pointer to the stack for validity */
		if (* spp != NULL) {

			/* free up the memory by passing in the address 
			 * of the stack*/
			/* memory keeps track of the address to the stack */
			void * memory = (* spp - STACK_OFFSET);
			free(memory);
			* spp = NULL;		/* set the pointer to NULL */
			stack_counter --;	/* decrement the counter */

			if (debug == TRUE) 
				/* prints DEBUG messages */
				fprintf(stderr, DEALLOCATE, (long)1);
		}
		else	
			/* prints ERROR messages*/
			fprintf(stderr, DELETE_NONEXIST);
	}
	else	
		fprintf(stderr, DELETE_NONEXIST); /* prints ERROR messages*/
}

/*--------------------------------------------------------------------------
Function Name:          empty_Stack
Purpose:                Empty the stack
Description:            Empty the stack by resetting the pointer index to 0. 
Input:                  this_Stack: the pointer to the current stack.
Result:                 The stack is emptied, the pointer index is 0.
			No return value
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {

	/* checks the stack pointer for validity */
	if (this_Stack != NULL) 
		this_Stack[STACK_POINTER_INDEX] = 0; /* set the index to 0 */
	else	
		fprintf(stderr, EMPTY_NONEXIST); /* prints ERROR messages */
}

/*--------------------------------------------------------------------------
Function Name:          isempty_Stack
Purpose:                Checks if the stack is empty.
Description:            Checks if the pointer index is 0;
Input:                  this_Stack: the pointer to the current stack.
Result:                 Return true if the stack is empty
			Return false if the stack is not empty
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {

	/* checks the stack pointer for validity */
	if (this_Stack != NULL) {

		/* returns true if the pointer index is 0,
		 * otherwise returns false*/
		if (this_Stack[STACK_POINTER_INDEX] == 0)	
			return 1;	/* return true */
		else 
			return 0;	/* return false */
	}
	else {
		fprintf(stderr, ISEMPTY_NONEXIST); /* prints ERROR messages */
		return 0;		/* return false */
	}
}

/*--------------------------------------------------------------------------
Function Name:          isfull_Stack
Purpose:                Checks if the stack is full.
Description:            Checks if the pointer index equals to the stack size;
Input:                  this_Stack: the pointer to the current stack.
Result:                 Return true if the stack is full.
			Return false if the stack is not full.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
	
	/* checks the stack pointer for validity */
	if (this_Stack != NULL) {

		/* returns true if the pointer index equals the stack size,
		 * otherwise returns false*/
		if (	this_Stack[STACK_POINTER_INDEX] >= 
			this_Stack[STACK_SIZE_INDEX]) 
			return 1;	/* return true */
		else 
			return 0;	/* return false */
	}
	else {
		fprintf(stderr, ISFULL_NONEXIST); /* prints ERROR messages */
		return 0;		/* return false */
	}
}

/*--------------------------------------------------------------------------
Function Name:          new_Stack
Purpose:                Create a new stack
Description:            Calls malloc with the size of the new stack to 
			be allocated. 
Input:                  stacksize: the size of the new stack to be allocated.
Result:                 A new stack is created
			Return the pointer to the new stack.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {

	/* memory keeps track of the pointer returned by malloc
	 * STACK_OFFSET is for the extra 3 hidden slots that keeps track of 
	 * the information of the stack*/ 
	void * memory = malloc ((stacksize + STACK_OFFSET) * sizeof(long));

	/* to return the address to caller */
	Stack * this_stack = (Stack *)memory + STACK_OFFSET;

 	/* initialization */
	stack_counter ++;	/* increment the stack counter*/
	this_stack[STACK_COUNT_INDEX] = stack_counter;
	this_stack[STACK_SIZE_INDEX] = stacksize;
	this_stack[STACK_POINTER_INDEX] = 0;

	if (debug == TRUE)	
		/* prints DEBUG messages */
		fprintf(stderr, ALLOCATED, (long)stack_counter);

	return this_stack;
}

/*--------------------------------------------------------------------------
Function Name:          get_occupandy
Purpose:                Checks the number of items in the stack.
Description:            Checks the ponter index for the number of items.
Input:                  this_Stack: the pointer to the current stack.
Result:         	Return the number of items in the stack.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
long get_occupancy (Stack * this_Stack) {

	/* checks the stack pointer for validity */
	if (this_Stack != NULL) 
		return this_Stack[STACK_POINTER_INDEX]; /* number of items*/
	else {
		fprintf(stderr, NUM_NONEXIST); /* prints ERROR messages */
		return 0;	/* return false*/
	}
}

/*--------------------------------------------------------------------------
Function Name:          pop
Purpose:                Pass the item on the top of the stack to main and 
			remove it from the stack
Description:            Pass the item and decrement the pointer index
Input:                  this_Stack: the pointer to the current stack.
			item: the pointer to the item
Result:         	Return the item at the top of the stack.
			Return true if pop is successful.
			Return false if pop is unsuccessful.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {

	/* checks the stack pointer for validity */
	if (this_Stack != NULL) {

		/* checks the item pointer for validity */
		if (item != NULL) {

			/* pop the item if the stack is not empty, 
			 * otherwise prints an error message.*/
			if (!isempty_Stack(this_Stack)){	
				long stack_pointer;	/* local pointer index*/
				stack_pointer = this_Stack[STACK_POINTER_INDEX];

				/* keeps track of the item to be popped */
				* item = this_Stack[stack_pointer - 1]; 

				/* decrement the pointer index*/
				this_Stack[STACK_POINTER_INDEX] --; 
				
				if (debug == TRUE) { 
					/*prints DEBUG messages*/
					fprintf(stderr, POP,
					(long)stack_counter, *item);
				}
				return 1;
			}
			else {	
				/* prints ERROR messages*/
				fprintf(stderr, POP_EMPTY);
				return 0;
			}
		}
		else {	
			/* prints ERROR messages*/
			fprintf(stderr, INCOMING_NONEXIST);
			return 0;
		}
	}
	else {	
		fprintf(stderr, POP_NONEXIST); /* prints ERROR messages*/
		return 0;
	}
}

/*--------------------------------------------------------------------------
Function Name:          push
Purpose:                Add the item to the top of the stack.
Description:            Add the item and increment the pointer index
Input:                  this_Stack: the pointer to the current stack.
			item: the pointer to the item
Result:         	The item is added to the stack
			Return true if puch is successful.
			Return false if puch is unsuccessful.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {

	/* checks the stack pointer for validity */
	if (this_Stack != NULL) {

		/* puch the item if the stack is not full, 
		* otherwise prints an error message.*/
		if (!isfull_Stack(this_Stack)) {
			long stack_pointer;	/* local stack pointer */
			stack_pointer = this_Stack[STACK_POINTER_INDEX];

			/* adds the item on the stack*/
			this_Stack[stack_pointer] = item;

			/* increment the pointer index */
			this_Stack[STACK_POINTER_INDEX] ++;
			
			if (debug == TRUE) { 
				/*prints DEBUG messages*/
				fprintf(stderr, PUSH, 
				(long)stack_counter, item);
			}
			return 1;
		}
		else { 
			fprintf(stderr, PUSH_FULL); /* prints ERROR messages*/
			return 0;
		}
	}
	else { 
		fprintf(stderr, PUSH_NONEXIST); /* prints ERROR messages*/
		return 0;
	}
}

/*--------------------------------------------------------------------------
Function Name:          top
Purpose:                Pass the item on the top of the stack to main without 
			removing it from the stack
Description:            Pass the item 
Input:                  this_Stack: the pointer to the current stack.
			item: the pointer to the item
Result:         	Return the item at the top of the stack.
			Return true if top is successful.
			Return false if top is unsuccessful.
			Prints error messages if the pointer is not valid.
--------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {

	/* checks the stack pointer for validity */
	if (this_Stack != NULL) {

		/* checks the item pointer for validity */
		if (item != NULL) {

			/* top the item if the stack is not empty, 
			 * otherwise prints an error message.*/
			if (!isempty_Stack(this_Stack)){	
				long stack_pointer;	/* local pointer index*/
				stack_pointer = this_Stack[STACK_POINTER_INDEX];

				/* set the deferenced item variable to the
				 * value of the stack */
				/* keeps track of the item to be popped */
				* item = this_Stack[stack_pointer - 1];
				
				if (debug == TRUE) { /*prints DEBUG messages*/
					fprintf(stderr, TOP,
					(long)stack_counter, *item);
				}
				return 1;
			}
			else {  
				/* prints ERROR messages*/
				fprintf(stderr, TOP_EMPTY);
				return 0;
			}
		}
		else {  
			/* prints ERROR messages*/
			fprintf(stderr, INCOMING_NONEXIST);
			return 0;
		}
	}
	else {  
		fprintf(stderr, TOP_NONEXIST);  /* prints ERROR messages*/
		return 0;
	}
}

FILE * write_Stack (Stack * this_Stack, FILE * stream) {

        long index = 0;         /* index into the stack */

        if (this_Stack == NULL) {
                fprintf (stderr, WRITE_NONEXIST_STACK);
                return stream;
        }

        if (stream == NULL) {
                fprintf (stderr, WRITE_NONEXIST_FILE);
                return stream;
        }
                
        if (stream == stderr)
                fprintf (stream, "Stack has %ld items in it.\n",
                        get_occupancy (this_Stack));

        for (index = STACK_COUNT_INDEX + STACK_OFFSET;
                index < get_occupancy (this_Stack); index++) {

                if (stream == stderr)
                        fprintf (stream, "Value on stack is |0x%lx|\n",
                                this_Stack[index]);
                else {
                        if (this_Stack[index] < 0)
                                fprintf (stream, "%c ",
                                        (char) this_Stack[index]);
                        else
                                fprintf (stream, "%ld ", this_Stack[index]);
                }
        }

        return stream;
}
