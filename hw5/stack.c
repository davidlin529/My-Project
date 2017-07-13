/******************************************************************************

							John David Lin
							CSE 12, Spring 2017
							May 2, 2017
							cs12xkz
				Assignment Five
File Name:	stack.c
Description:	This program delagates its functions to list.c
*******************************************************************************/


#include <stdio.h>
#include "list.h"
#include "stack.h"

/*--------------------------------------------------------------------------
Function Name:          delete_Stack 
Purpose:		Delete a list
Description:            Call delte_list to delete a list.
Input:                  spp: pointer to pointer to a stack
Result:                 A list is deleted. 
			No return value
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
        delete_List (spp);
}

/*--------------------------------------------------------------------------
Function Name:          isempty_Stack
Purpose:                Check is the list is empty.
Description:		Call isempty_List to check to see if this_list is 
			empty.
			
Input:                  this_stack: the pointer to the stack

Result:                 Return TRUE if the list is empty
			Return FALSE otherwise
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_stack) {
        return isempty_List (this_stack);
}

/*--------------------------------------------------------------------------
Function Name:          new_Stack
Purpose:                Create a new list
Description:            Call new_List to create a new list

Input:                  copy_func: the pointer to copy_func function
			delete_func: the pointer to delete_func function
			is_greater_than_func: the pointer to 
				is_greater_than_func function
			write_func: the pointer to write_func function

Result:                 A new list is initialized and returned.
--------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        return new_List (copy_func, delete_func, 
		is_greater_than_func, write_func);
}

/*--------------------------------------------------------------------------
Function Name:          pop
Purpose:                Remove a node from the list
Description:            Call remove_list to remove an element in this list.
Input:			this_stack: the pointer to the stack.

Result:                 A node is removed from the list.
			No return value.
--------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
        return remove_List (this_stack, END);
}

/*--------------------------------------------------------------------------
Function Name:          push
Purpose:                insert an item into the list
Description:            Call insert to inserts the element into this_stack. 

Input:                  this_stack: the pointer to the stack.
			element: the pointer to te item to insert

Result:                 Return TRUE if the insertion is successful
			Return FALSE otherwise
--------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
        return insert (this_stack, element, END);
}

/*--------------------------------------------------------------------------
Function Name:          top
Purpose:                View an item in the list
Description:            Call view to view an item in the list.
Input:			this_stack: the pointer to the stack.

Result:                 A node is viewed in the list.
			No return value.
--------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
        return view (this_stack, END);
}

/*--------------------------------------------------------------------------
Function Name:          write_Stack
Purpose:                Write the list
Description:            Call write_list to write the elements of this_list.
Input:			this_stack: the pointer to the stack.
			stream: the stream in which text is printed.

Result:                 A list is written.
			Return stream.
--------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_stack, FILE * stream) {
        return write_List (this_stack, stream);
}
