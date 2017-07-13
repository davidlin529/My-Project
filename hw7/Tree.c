/******************************************************************************
							John David Lin
							CSE 12, Spring 2017
							May 15, 2017
							cs12xkz
				Assignment Seven
File Name:	Tree.c
Description:	This program creates a tree and allows operations of insert,
		lookup, and remove on TNodes on the tree. 
*******************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";

struct TNode {
	Base * data;
	TNode * left, * right, *parent;
	static long occupancy;
        unsigned long hasBeenDeleted;

	// left child's height - right child's height
	long balance;

	// 1 + height of tallest child, or 0 for leaf
	long height;

	TNode (Base * element) : data (element), left (0), right (0),
	        parent (0), hasBeenDeleted (FALSE), balance (0), height(0) {
		
		occupancy ++;  // increase occupancy count by 1
	}
	~TNode (void) {
		delete (data);	// deallocates the node
		left = right = 0; // set the pointers to 0
	}
        ostream & Write (ostream & stream) const {
                stream << "at height:  " << height << "  with balance:  "
			<< balance << "  ";
		return data->Write (stream) << "\n";
        }
	ostream & Write_AllTNodes (ostream & stream) const {
		if (left)
			left->Write_AllTNodes (stream);
		if (!hasBeenDeleted)
			Write (stream);
		if (right)
			right->Write_AllTNodes (stream);

		return stream;
	}

	/* deallocates all TNodes */
	void delete_AllTNodes (void) {
		/* check left */
		if (left)
			left->delete_AllTNodes();

		/* check right */
		if (right)
			right->delete_AllTNodes();

		/* deallocate the node */
		delete (this);
		return;
	}
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;

void Tree :: Set_Debug (bool option) {
/***************************************************************************
% Routine Name : Tree :: Set_Debug (public)
% File :         Tree.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/
	if (option)
		debug_on = true;
	else
		debug_on = false;
}

Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes root pointer to NULL, and occupancy to 0.
***************************************************************************/

{
	TNode :: occupancy = 0;
	/* debug messages */
	if (debug_on) 
		cerr << TREE << tree_name << ALLOCATE;
}

Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
	/* delete all the nodes */
	if (TNode :: occupancy != 0)
		root->delete_AllTNodes();
	
	/* deallocate tree name */ 
	free ((void *)tree_name);

	/* debug messages */
	if (debug_on) 
		cerr << TREE << tree_name << DEALLOCATE;
}	/* end: ~Tree */

unsigned long Tree :: Insert (Base * element) {
/***************************************************************************
% Routine Name : Tree :: Insert  (public)
% File :         Tree.c
% 
% Description :  Insert an element into the tree

Paramters descriptions :

name			description
------------------ ------------------------------------------------------
element			the element to insert
***************************************************************************/
	TNode * working;
	working = root;	// the current TNode

	/* check for first item insertion */
	if (!root) {
		working = root = new TNode(element); //insert at root

		/* debug messages */
		if (debug_on) 
			cerr << TREE << tree_name << INSERT << 
		(const char *) *element << "]\n";
	}

	/* insert somewhere down the tree */
	else while (true) {

		/* debug messages */
		if (debug_on)
			cerr << TREE << tree_name << COMPARE << 
			(const char *) *element << AND << 
			(const char *) *working->data << "]\n";

		/* check for duplicate */
		if (*element == *(working->data)) {
			working->data = element;
			if (working->hasBeenDeleted == true) {
				working->hasBeenDeleted = false;
				TNode :: occupancy ++;
			}
			break;
		}

		/* check to go right */
		if (*element > *(working->data)) {
			/* go right */
			if (!working->right) {

				/* debug messages */
				if (debug_on)
					cerr << TREE << 
					tree_name << INSERT << 
					(const char *) *element << "]\n";

				/* insert the node and update the 
				 * parent and working pointers 
				 */
				working->right = new TNode(element);
				working->right->parent = working;
				working = working->right;
				break;
			}

			/* insert somewhere down the right */
			else 
				working = working->right;
		}
			
		/* check to go left */
		else {				
			/* go left */
			if (!working->left) {

				/* debug messages */
				if (debug_on)
					cerr << TREE <<
					tree_name << INSERT <<
					(const char *) *element << "]\n";
					
				/* insert the node and update the 
				 * parent and working pointers 
				 */
				working->left = new TNode(element);
				working->left->parent = working;
				working = working->left;
				break;
			}
			
			/* inset somewhere down the let */
			else
				working = working->left;
		}
	}

	/* travel back to the parent nodes to set 
	 * heights and balances
	 */
	while (working->parent) {
		working = working->parent;  // go up to the parent
		long leftHeight = -1;	// height of the left child
		long rightHeight = -1;	// height of the right child

		/* set leftHeight */
		if (working->left)
		leftHeight = working->left->height;

		/* set rightHeight */
		if (working->right)
			rightHeight = working->right->height;

		/* height = tallest child + 1 */
		if (leftHeight > rightHeight)
			working->height = leftHeight + 1;
		else
			working->height = rightHeight + 1;

		/* set balance */
		working->balance = leftHeight - rightHeight;
	}
		
	return true;
}

const Base * Tree :: Lookup (const Base * element) const {
/***************************************************************************
% Routine Name : Tree :: Lookup  (public)
% File :         Tree.c
% 
% Description :  Look up an item from the tree

Paramters descriptions :

name			description
------------------ ------------------------------------------------------
element			the element to lookup
***************************************************************************/
	TNode * working;
	working = root;	// the current TNode

	/* check if the root is null */
	if (!root) {
		return NULL;
	}
		
	/* for nodes other than the first one */
	else while (true) {
		
		/* debug messages */
		if (debug_on)
			cerr << TREE << tree_name << COMPARE <<
			(const char *) *element << AND <<
			(const char *) *working->data<< "]\n";

		/* check for the same item */
		if (*element == *(working->data)) {
				
			/* if the item has't been deleted */
			if (!(working->hasBeenDeleted))
				return working->data;

			/* item has been deleted */
			else 
				return NULL;
		}
			
		/* checks to the right */
		if (*element > *(working->data)) {

			/* return null if there's no more item */
			if (!(working->right)) 
				return NULL;
					
			/* go right */
			else
				working = working->right;
		}
		
		/* check to go left */
		else {

			/* return null if there's no more item */
			if (!(working->left))
				return NULL;

			/* go left  */
			else
				working = working->left;
		}
	}
}

Base * Tree :: Remove (const Base * element) {
/***************************************************************************
% Routine Name : Tree :: Remove  (public)
% File :         Tree.c
% 
% Description :  Remove an item from the tree

Paramters descriptions :

name			description
------------------ ------------------------------------------------------
element			the element to remove
***************************************************************************/
	TNode * working;
	working = root;	// the current TNode

	/* check if the root is null */
	if (!root) {
		return NULL;
	}
		
	/* for nodes other than the first one */
	else while (true) {
		
		/* debug messages */
		if (debug_on)
			cerr << TREE << tree_name << COMPARE <<
			(const char *) *element << AND <<
			(const char *) *working->data<< "]\n";

		/* check for the same item */
		if (*element == *(working->data)) {
				
			/* if the item has't been deleted */
			if (!(working->hasBeenDeleted)) {
				working->hasBeenDeleted = true;
				TNode :: occupancy --;
				return working->data;
			}

			/* item has been deleted */
			else 
				return NULL;
		}
			
		/* checks to the right */
		if (*element > *(working->data)) {

			/* return null if there's no more item */
			if (!(working->right)) 
				return NULL;
					
			/* go right */
			else
				working = working->right;
		}
		
		/* check to go left */
		else {

			/* return null if there's no more item */
			if (!(working->left))
				return NULL;

			/* go left  */
			else
				working = working->left;
		}
	}
}

ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */

