/******************************************************************************

							John David Lin
							CSE 12, Spring 2017
							May 2, 2017
							cs12xkz
				Assignment Five
File Name:	list.c
Description:	This program has a polymorphic generic
		container based on a linked-list data structure. In list.c, 
		the List and Node structs are defined. The List object contains
		data fields such as occupancy and list_count, and also contains 
		pointers to functions which manipulate the objects stored in the
		list. The Node objects form a doubly-linked list, where the pre 
		pointer points to the previous item in the list and the next 
		pointer points to the next item in the list. The list is 
		circular, so the next pointer of the last item points to the 
		first item of the list, and the pre pointer of the first item 
		points to the last item in the list. We keep track of the front 
		item in the list with the front data field of List.

*******************************************************************************/
/******************************************************************************
Debugger questions:
1. front: 0x604070
2. Node 1 data: 0x604050
3. Node 1 pre: 0x6040b0
4. Node 1 next: 0x6040b0
5. Node 2 data: 0x604090
6. Node 2 pre: 0x604070
7. Node 2 next: 0x6040f0
8. Node 3 data: 0x6040d0
9. Node 3 pre: 0x6040b0
10. Node 3 next: 0x604070
*******************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
        struct Node * pre;      /* how to access the prior Node */
        struct Node * next;     /* how to access the next Node */
        void * data;            /* the data to store */
} Node;

typedef struct List {
        Node * front;             /* the front of the list */
        long list_count;        /* which list is it */
        long occupancy;
        void *(*copy_func) (void *);
        void (*delete_func) (void *);
        long (*is_greater_than_func) (void *, void *);
        FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static void delete_Node (Node **, void (*delete_func) (void *));
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static Node * locate (List * this_list, void * element);
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, FILE *(*write_func) (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] =	
                "Advance next from non-existent list!!!\n";	
static const char ADNEXT_EMPTY[] =				
                "Advance next from empty list!!!\n";	
static const char ADPRE_NONEXIST[] =			
                "Advance pre from non-existent list!!!\n";	
static const char ADPRE_EMPTY[] =			
                "Advance pre from empty list!!!\n";		
static const char DELETE_NONEXIST[] =				
                "Deleting from non-existent list!!!\n";		
static const char DELETE_NONEXISTNODE[] =			
                "Deleting a non-existent node!!!\n";		
static const char ISEMPTY_NONEXIST[] =		
                "Is empty check from non-existent list!!!\n";	
static const char INSERT_NONEXIST[] =				
                "Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =			
                "Removing from non-existent list!!!\n";		
static const char REMOVE_EMPTY[] =				
                "Remove from empty list!!!\n";		
static const char VIEW_NONEXIST[] =				
                "Viewing a non-existent list!!!\n";		
static const char VIEW_NONEXISTNODE[] =				
                "Viewing a non-existent node!!!\n";		
static const char VIEW_EMPTY[] =				
                "Viewing an empty list!!!\n";			
static const char WRITE_NONEXISTFILE[] =
                "Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
                "Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
                "Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
                "Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";	
static const char INSERT[] = "[List %ld - Inserting node]\n";	
static const char REMOVE[] = "[List %ld - Removing node]\n";	
static const char VIEW[] = "[List %ld - Viewing node]\n";	
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n"; 

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */

/*--------------------------------------------------------------------------
Function Name:          set_debug_on
Purpose:                Turn debug on 
Description:            Set debug_on to TRUE
Input:                  No input
Result:                 Debug is on.
			No return value
--------------------------------------------------------------------------*/
void set_debug_on (void) {
	debug_on = TRUE;
}

/*--------------------------------------------------------------------------
Function Name:          set_debug_off
Purpose:                Turn debug off.
Description:            Set debug_on to FALSE
Input:                  No input
Result:                 Debug is off.
			No return value
--------------------------------------------------------------------------*/
void set_debug_off (void) {
	debug_on = FALSE;
}

/*--------------------------------------------------------------------------
Function Name:          advance_next_List
Purpose:                Shift the front pointer forward
Description:            Calling this function causes the front pointer of 
			this_list to move forward by one Node. This effectively
			shifts the elements of the list forward by one.
Input:                  this_list: the pointer to the list.
Result:                 The front pointer is shifted forward.
			No return value
--------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
	/* validity check */
	if (!this_list)
		fprintf(stderr, ADNEXT_NONEXIST);

	/* check if the list is empty */
	if (!isempty_List(this_list)) {
		/* shift the pointer */
		this_list -> front = this_list -> front -> next;
		/* debug messages */
		if (debug_on)
			fprintf(stderr, ADNEXT, this_list -> list_count);
	}

	/* error messages */
	else {
		fprintf(stderr, ADNEXT_EMPTY);
	}
}

/*--------------------------------------------------------------------------
Function Name:          advance_pre_List
Purpose:                Shift the front pointer backward
Description:            Calling this function causes the front pointer of 
			this_list to move backward by one Node. This effectively
			shifts the elements of the list backwards by one.
Input:                  this_list: the pointer to the list.
Result:                 The front pointer is shifted backward.
			No return value
--------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {
	/* validity check */
	if (!this_list) 
		fprintf(stderr, ADNEXT_NONEXIST);
	
	/* check if the list is empty */
	if (!isempty_List(this_list)) {
		/* shift the pointer */
		this_list -> front = this_list -> front -> pre;
		/* debug messages */
		if (debug_on)
			fprintf(stderr, ADPRE, this_list -> list_count);
	}
	/* error messages */
	else {
		fprintf(stderr, ADPRE_EMPTY);
	}
}

/*--------------------------------------------------------------------------
Function Name:          delete_List
Purpose:                Deallocate the list
Description:            This destructor function deallocates all memory 
			associated with the list, including the memory 
			associated with all of the nodes in the list. It sets 
			the list pointer in the calling function to NULL.
Input:                  List: the pointer to pointer to the list.
Result:                 The list is deallocated.
			No return value
--------------------------------------------------------------------------*/
void delete_List (List ** lpp) {
	long count;		/* keeps track of each node */
	Node * working;		/* pointer to current node */
	List * this_list;	/* represents ** lpp */
	this_list = *lpp;

	/* validity check */
	if (!this_list) {
		fprintf(stderr, DELETE_NONEXISTNODE);
		return;
	}
	
	/* deallocate each node of the list */
	for (count = 1; count <= this_list -> occupancy; count ++) {
		working = this_list -> front -> pre; /* shift the pointer */
		delete_Node(&(this_list -> front), this_list -> delete_func);
		this_list -> front = working;	/* change the front pointer */
	}
	
	list_counter --; /* decrement */
	
	free((void *) this_list);	/* call free to free up memory */
	this_list = NULL;		/* remove access */
	/* debug messages */
	if (debug_on)
		fprintf(stderr, LIST_DEALLOCATE, list_counter + 1);
}

/*--------------------------------------------------------------------------
Function Name:          insert 
Purpose:                insert an item into the list
Description:            Inserts the element into this_list either at the front, 
			end or in sorted form. locate should be used to locate 
			where the node should go in the list.
Input:                  this_list: the pointer to the list
			element: the pointer to te item to insert
			where: location of insertion
Result:                 Return TRUE if the insertion is successful
			Return FALSE otherwise
--------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) {
	/* validity check */
	if (this_list) {
		Node * this_Node = 0;	/* keeps track of the current node */

		/* if there already exists a node */
		if (this_list -> front) {
			/* set the current node to the front node */
			if ((where == FRONT) || (where == END)) {
				this_Node = this_list -> front;
			}
			/* set the node based on locate */
			else if (where == SORTED) {
				this_Node = locate(this_list, element);
			}
		}
		
		/* insert the mode */
		this_Node = insert_Node(
			this_Node, element, this_list -> copy_func);

		/* update the front poiter */
		if (where == FRONT)
			this_list -> front = this_Node;
		this_list -> occupancy ++;	/* increment */

		/* debug messages */
		if (debug_on)
			fprintf(stderr, INSERT, this_list -> list_count);

		/* set the front pointer in the case of the first node */
		if (!(this_list -> front))
			this_list -> front = this_Node;

		/* change the pointer to the smaller value in the case of 
		 * locate where the inserted value is larger
		 */
		if ((where == 2) && this_list -> is_greater_than_func(
			this_list -> front -> data, this_Node -> data)){
			this_list -> front = this_Node;
		}
		return TRUE;
	}
	else {
		fprintf(stderr, INSERT_NONEXIST); /* error messages */
		return FALSE;
	}
}

/*--------------------------------------------------------------------------
Function Name:          locate
Purpose:                Find the locati of insertion of an item into the 
			list.
			
Description:            Locate should be used to locate where the node should 
			be in a sorted list. If your implementation of locate 
			changes the front pointer of the list, be sure to 
			save that value before calling this function.

Input:                  this_list: the pointer to the list
			element: the pointer to te item to insert

Result:                 Return the pointer of the exiting node in which 
			a new one is to be inserted.
--------------------------------------------------------------------------*/
static Node * locate (List * this_list, void * element) {
	Node * front_Node = this_list -> front;	/* keep track of front node */
	Node * this_Node = this_list -> front;	/* keep track of working node*/
	
	/* find the location to be inserted */
	while(TRUE) {
		/* shift the poitner until the element value is smaller than 
		 * the working node's 
		 */
		if (this_list->is_greater_than_func(element,this_Node-> data)) {
			this_Node = this_Node -> next; 
		}
		else {
			return this_Node;
		}
		/* break if the entire list has been searched */
		if (this_Node == front_Node) {
			return this_Node;
		} 
	}
}

/*--------------------------------------------------------------------------
Function Name:          isempty_List
Purpose:                Check is the list is empty.
Description:            Checks to see if this_list is empty.
Input:                  this_list: the pointer to the list

Result:                 Return TRUE if the list is empty
			Return FALSE otherwise
--------------------------------------------------------------------------*/
long isempty_List (List * this_list) {
	/* validity check */
	if (this_list) {

		/* check if the front pointer is null and occupancy is 0 */
		if (!(this_list -> front) && !(this_list -> occupancy)) {
			return TRUE;
		}
		else {
			return FALSE;
		}
	}

	/* error messages */
	else {
		fprintf(stderr, ISEMPTY_NONEXIST);
		return FALSE; 
	}
}

/*--------------------------------------------------------------------------
Function Name:          new_List
Purpose:                Create a new list

Description:            This constructor function allocates and initializes 
			a new List object. It initializes the list data 
			fields, and returns a pointer to the list. All new 
			lists should be empty.
			
Input:                  copy_func: the pointer to copy_func function
			delete_func: the pointer to delete_func function
			is_greater_than_func: the pointer to 
				is_greater_than_func function
			write_func: the pointer to write_func function

Result:                 A new list is initialized and returned.
--------------------------------------------------------------------------*/
List * new_List (
        void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        /* allocate */
	List * this_list = (List *) malloc(sizeof(List));	

	/* initialize each field of the list */
	this_list -> front = NULL;
	this_list -> occupancy = 0;
	this_list -> list_count = list_counter;
	this_list -> list_count ++;
	this_list -> copy_func = copy_func;
	this_list -> delete_func = delete_func;
	this_list -> is_greater_than_func = is_greater_than_func;
	this_list -> write_func = write_func;

	list_counter ++; /* increment the counter */
	
	/* debug messages */
	if (debug_on)
		fprintf(stderr, LIST_ALLOCATE, this_list -> list_count);
	return this_list;
}

/*--------------------------------------------------------------------------
Function Name:          remove_List
Purpose:                Remove a node from the list
Description:            Removes an element in this_list at location where. 
			This function should call remove_Node to restructure 
			the list to remove the node.
Input:			this_list: the pointer to the list.
			where: where the pointer will be removed

Result:                 A node is removed from the list.
			No return value.
--------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) {
	void * data;		/* stores the pointer to the data */
	Node * this_Node;

	/* validity check */
	if (!this_list) {
		fprintf(stderr, REMOVE_NONEXIST);
		return NULL;
	}

	/* check if the list is empty */
	if (isempty_List(this_list)) {
		fprintf(stderr, REMOVE_EMPTY);
		return NULL;
	}
	
	/* in the case of only 1 node */
	if (this_list -> occupancy == 1) {
		this_Node = this_list -> front;
		this_list -> front = NULL; /* set front pointer to null */
	}
	
	/* removing from the front */
	else if (where == 1) {
		/* change the front poitner */
		this_list -> front = this_list -> front -> next;
		this_Node = this_list -> front -> pre;
	}

	/* removing from the end */
	else if (where == 0) {
		this_Node = this_list -> front -> pre;
	}

	data = remove_Node(this_Node);	/* remove the node */
	this_list -> occupancy --;	/* decrement */

	/* debug messages */
	if (debug_on)
		fprintf(stderr, REMOVE, this_list -> list_count);
	
	return data;
}

/*--------------------------------------------------------------------------
Function Name:          view
Purpose:                View an item in the list
Description:            Returns a pointer to the object stored at location 
			where for viewing. This function should call 
			view_Node, which returns the data.
Input:			this_list: the pointer to the list.
			where: where the pointer will be viewed

Result:                 A node is viewed in the list.
			No return value.
--------------------------------------------------------------------------*/
void * view (List * this_list, long where) {
	void * data;		/* stores the pointer to the data */
	Node * this_Node;

	/* validity check */
	if (!this_list) {
		fprintf(stderr, VIEW_NONEXIST);
		return NULL;
	}

	/* check if the list is empty */
	if (isempty_List(this_list)) {
		fprintf(stderr, VIEW_EMPTY);
		return NULL;
	}

	/* viewing from the front */
	if (where == 1)  {
		this_Node = this_list -> front;
	}

	/* viewing from the end */
	else if (where == 0) {
		this_Node = this_list -> front -> pre;
	}

	data = view_Node(this_Node);

	/* debug messages */
	if (debug_on)
		fprintf(stderr, VIEW, this_list -> list_count);
		
	return data;
}

FILE * write_List (List * this_list, FILE * stream) {
        long count;             /* to know how many elements to print */
        Node * working;                 /* working node */

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }

        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);

        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        if (this_list->occupancy >= 1)
                working = this_list->front;

        for (count = 1; count <= this_list->occupancy; count++) {
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->next;
        }
                
        return stream;
}

/*--------------------------------------------------------------------------
Function Name:          write_reverse_List
Purpose:                Write the list in reverse.
Description:            Writes the elements of this_list backwards, starting 
			with the last item. The list is printed to filestream 
			stream.
Input:			this_list: the pointer to the list.
			stream: the stream in which text is printed.

Result:                 A list is written in reverse
			Return stream.
--------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) {
	long count;             /* to know how many elements to print */
        Node * working;                 /* working node */
	
	/* validity check */
        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }
	
	/* validity check */
        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }

	/* write the number of items in the list */
        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);
	
	/* validity check for write_func */
        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

	/* set the working pointer */
        if (this_list->occupancy >= 1)
                working = this_list->front->pre;
	
	/* going through the list to print each node */
        for (count = 1; count <= this_list->occupancy; count++) {
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->pre; /* shift the poitner */
        }
                
        return stream;
}

static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

        /* does the node exist??? */
        if (!npp || !*npp) {
                fprintf (stderr, DELETE_NONEXISTNODE);
                return;
        }

        /* call function to delete element */
        if (delete_func && (*npp)->data)
                (*delete_func) (&((*npp)->data));

        /* delete element */
        free (*npp);

        /* assign node to NULL */
        *npp = NULL;
}

/*--------------------------------------------------------------------------
Function Name:          insert _Node
Purpose:                insert a node into the list

Description:            Creates a new node to hold element, or, if copy_func 
			is non-NULL, a copy of element. This new node is then 
			incorporated into the list at the location BEFORE 
			this_Node.

Input:                  this_list: the pointer to the list
			element: the pointer to te item to insert
			copy_func: the pointer to copy_func function

Result:                 Return the pointer to the node in which a new one
			will be inserted.
--------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
        void * (*copy_func) (void *)) {

	/* for the case of the first node in the list */
	if (!this_Node) {
		this_Node = new_Node(element, copy_func);
		/* initialize the pointers */
		this_Node -> pre = this_Node -> next = this_Node;
		return this_Node;
	}
	/* for the case of non first node in the list */
	else {
		Node * working = new_Node(element, copy_func);
		/* attach the node */
		working -> pre = this_Node -> pre;
		working -> next = this_Node;
		/*integrate the node */
		this_Node -> pre -> next = working;
		this_Node -> pre = working;
		return working;
	}
}

static Node* new_Node (void * element, void * (*copy_func) (void *)) {

        /* allocate memory */
        Node *this_Node = (Node *) malloc (sizeof (Node));

        /* initialize memory */
        this_Node->next = this_Node->pre = NULL;
        this_Node->data = (copy_func) ? (*copy_func) (element) : element;

        return this_Node;
}

/*--------------------------------------------------------------------------
Function Name:		remove_Node
Purpose:                Remove a node from the list.

Description:            "Unlinks" this_Node from the list by arranging the 
			pointers of the surrounding Nodes so they no longer 
			point to this_Node. The memory associated with the 
			Node object is freed, but the Node's data is not 
			deleted. A pointer to the data is returned.

Input:                  this_Node: the pointer to the node to be removed.
Result:                 The Node is removed.
			No return value.
--------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) {
	void * data;	/* stores the pointer to the data */

	/* detach the pointers to the node */
	this_Node -> pre -> next = this_Node -> next;
	this_Node -> next -> pre = this_Node -> pre;
	
	data = this_Node -> data;	/* stores the data */
	delete_Node(&this_Node, 0);	/* deallocate the node */
	return data;
}

/*--------------------------------------------------------------------------
Function Name:		view_Node
Purpose:                View a node from the list.

Description:            Returns a pointer to this_Node's data.

Input:                  this_Node: the pointer to the node to be removed.
Result:                 The Node is viewed.
			No return value.
--------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) {
	/* validity check */
	if (this_Node){
		return this_Node -> data;
	}
	else {
		/* error messages */
		fprintf(stderr, VIEW_NONEXISTNODE);
		return NULL;
	}
}

static FILE* write_Node (Node * this_Node, FILE * stream,
        FILE * (*write_func) (void *, FILE *)) {

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_Node) {
                fprintf (stream, WRITE_NONEXISTNODE);
                return stream;
        }

        if (!write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        return (*write_func) (this_Node->data, stream);
}
