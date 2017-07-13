/******************************************************************************
							John David Lin
							CSE 12, Spring 2017
							May 30, 2017
							cs12xkz
				Assignment Eight
File Name:	Tree.c
Description:	This program creates a tree and allows operations of insert,
		lookup, and remove on TNodes on the tree. 
*******************************************************************************/

#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char CHECK[] = " - Checking ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;	// determine if debug is on

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

/***************************************************************************
% Class Name : TNode
% File :	Tree.c
% 
% Description : This class constains TNode constructors, destructors, 
		and its own funcitons. TNodes hold data and can be 
		inserted into a Tree.
***************************************************************************/
template <class Whatever>
struct TNode {
        long balance;
        Whatever data;
        long height;
        TNode<Whatever> * left;
        long & occupancy;
        TNode<Whatever> * right;
        unsigned long & tree_count;
        
        TNode (const Whatever & element, Tree<Whatever> & theTree)
                : balance (0), data (element), height (0), left (0), 
                occupancy (theTree.occupancy), right (0), 
                tree_count (theTree.tree_count) {
                
		occupancy ++;	// increment occupancy
        }
        
        TNode (const Whatever & element, TNode<Whatever> & parentTNode)
        : balance (0), data (element), height (0), left (0), 
                occupancy (parentTNode.occupancy), right (0), 
                tree_count (parentTNode.tree_count) {
                
		occupancy ++;	// increment occupancy
        }

        ~TNode (void) {
                
		occupancy --;	// decrement occupancy
        }

/***************************************************************************
% Routine Name : delete_AllTNodes
% File :         Tree.c
% 
% Description :  This function deletes all the TNodes in the Tree
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% void            
***************************************************************************/
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

/***************************************************************************
% Routine Name : TNode<Whatever> :: Insert
% File :         Tree.c
% 
% Description :  This function inserts an element into another element 
		 using PointerInParent, delegated by Tree's Insert
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% const Whatever & element            the data to be stored in the TNode.
% TNode<Whatever> *& PointerInParent  a reference to the TNode pointer in 
					the parent TNode used to get to 
					the current TNode
***************************************************************************/
        unsigned long Insert (const Whatever & element, 
                              TNode<Whatever> *& PointerInParent);

        // OPTIONAL TNode :: Lookup => uncomment if implementing recursively
        // unsigned long Lookup(Whatever & element) const;

/***************************************************************************
% Routine Name : ReplaceAndRemoveMax
% File :         Tree.c
% 
% Description :  Called when removing a TNode with 2 children, replaces that 
		TNode with the maximum TNode in its left subtree to maintain 
		the Tree structure.
%
% Parameters descriptions :
% 
% name				description
% ------------------ ------------------------------------------------------
% targetTNode		a reference to the TNode to remove that has 2 children
% PointerInParent	a reference to the TNode pointer in the parent TNode 
			used to get to the current TNode.
***************************************************************************/
        void ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
                TNode<Whatever> *& PointerInParent) {
		
		/* debug messages */
		if (Tree<Whatever>::debug) 
			cerr << TREE << 
			tree_count << CHECK << 
			(const char *) data << "]\n";
		
		/* keep going to the right */
		if (PointerInParent->right)
			right -> ReplaceAndRemoveMax(targetTNode, right);
		
		/* reach the predecessor */
		else {
			targetTNode.data = data; // store predecessors' data
			
			/* reattach left child if any */
			if (left)
				PointerInParent = left;

			/* set the pointer to null */
			else 
				PointerInParent = NULL;

			delete(this); // deallocate the predecessor
			
			return;
		}

		/* debug messages */
		if (Tree<Whatever>::debug) 
			cerr << TREE << 
			tree_count << UPDATE << 
			(const char *) data << "]\n";

		/* call SHB */
		this -> SetHeightAndBalance(PointerInParent);

		return;
        }

/***************************************************************************
% Routine Name : Remove
% File :         Tree.c
% 
% Description :  Removes the matching data from the binary tree. Returns true 
%		or false indicating success of removal.
%
% Parameters descriptions :
% 
% name				description
% ------------------ ------------------------------------------------------
% elementTNode 		a reference to the TNode containing the data that 
			identifies the element to remove
% PointerInParent	a reference to the TNode pointer in the parent TNode 
			used to get to the current TNode.
***************************************************************************/
        unsigned long Remove (TNode<Whatever> & elementTNode, 
                TNode<Whatever> *& PointerInParent,
                long fromSHB = FALSE) {

		unsigned long status = FALSE; // hold the return status
		
		/* debug messages */
		if (Tree<Whatever>::debug)
			cerr << TREE << tree_count << COMPARE << 
			(const char *) elementTNode.data << AND << 
			(const char *) data << "]\n";

		/* check for the same item */
		if (elementTNode.data == data) {

			/* leaf nodes */
			if ((!left) && (!right)) {
				elementTNode.data = data; // store the data
				PointerInParent = NULL;   // reset pointer
				delete(this);		  // deallocate
				
				return TRUE;
			}
			
			/* nodes with only right child */
			else if ((!left) && right) {
				elementTNode.data = data; // store the data
				PointerInParent = right;  // reset pointer
				delete(this);		  // deallocate
				
				return TRUE;
			}

			/* nodes with only left child */
			else if (left && (!right)) {
				elementTNode.data = data; // store the data
				PointerInParent = left;	 // reset pointer
				delete(this);		 // deallocate
				
				return TRUE;
			}

			/* nodes with both left and right children */
			else {
				/* call RARM to return the predecessor */
				left -> ReplaceAndRemoveMax(elementTNode, left);
				
				/* replace the data with predecessor's */
				data = elementTNode.data;

				/* call SHAB */
				this -> SetHeightAndBalance(PointerInParent);

				/* debug messages */
				if (Tree<Whatever>::debug) 
					cerr << TREE << 
					tree_count << UPDATE << 
					(const char *) data << "]\n";
				
				return TRUE;
			}
		}

		/* check to go right */
		else if (elementTNode.data > data) {

			/* return null if there's no more item */
			if (!right) 
				return FALSE;

			/* insert somewhere down the right */
			else
				status = right -> Remove(elementTNode, right);
		}

		/* check to go left */
		else {

			/* return null if there's no more item */
			if (!left)
				return FALSE;

			/* insert somewhere down the left */
			else
				status = left -> Remove(elementTNode, left);
		}

		/* call SHAB if not called from it*/
		if (status && !fromSHB)
			this -> SetHeightAndBalance(PointerInParent);

		/* debug messages */
		if ((Tree<Whatever>::debug) && status && !fromSHB) 
			cerr << TREE << 
			tree_count << UPDATE << 
			(const char *) data << "]\n";
		
		return status;
        }

/***************************************************************************
% Routine Name : SetHeightAndBalance
% File :         Tree.c
% 
% Description :  Updates the height and balance of the current TNode.
%
% Parameters descriptions :
% 
% name				description
% ------------------ ------------------------------------------------------
% PointerInParent	a reference to the TNode pointer in the parent TNode 
			used to get to the current TNode.
***************************************************************************/
        void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) {
                long leftHeight = -1;	// height of the left child
		long rightHeight = -1;	// height of the right child
		TNode<Whatever> temp = *this; // hold the returned node

		/* set leftHeight */
		if (left)
			leftHeight = left->height;

		/* set rightHeight */
		if (right)
			rightHeight = right->height;

		/* height = tallest child + 1 */
		if (leftHeight > rightHeight)
			height = leftHeight + 1;
		else
			height = rightHeight + 1;

		/* set balance */
		balance = leftHeight - rightHeight;

		/* over the threshold */
		if (abs(balance) > THRESHOLD) {
			/* debug messages */
			if (Tree<Whatever>::debug) 
				cerr << TREE << 
				tree_count << REPLACE << 
				(const char *) data << "]\n";

			/* remove the one over threshold */
			this -> Remove(temp, PointerInParent, TRUE);

			/* reinsert into the tree */
			PointerInParent -> Insert(temp.data, PointerInParent);
		}
        }

        ostream & Write_AllTNodes (ostream & stream) const {
                if (left)
                        left->Write_AllTNodes (stream);
                stream << *this;
                if (right)
                        right->Write_AllTNodes (stream);

                return stream;
        }
};

/***************************************************************************
% Routine Name : Tree<Whatever> :: Set_Debug_On
% File :         Tree.c
% 
% Description :  Set debug on
%
% Parameters descriptions :
% 
% name				description
% ------------------ ------------------------------------------------------
% void
***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On (void) {
	debug = TRUE; // set debug to true;
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Set_Debug_Off
% File :         Tree.c
% 
% Description :  Set debug off
%
% Parameters descriptions :
% 
% name				description
% ------------------ ------------------------------------------------------
% void
***************************************************************************/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off (void) {
	debug = FALSE; // set debug to false;
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: Insert
% File :         Tree.c
% 
% Description :  This function inserts an element into another element 
		 using PointerInParent, delegated by Tree's Insert
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% const Whatever & element            the data to be stored in the TNode.
% TNode<Whatever> *& PointerInParent  a reference to the TNode pointer in 
					the parent TNode used to get to 
					the current TNode
***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
                                         TNode<Whatever> *& PointerInParent) {

	/* debug messages */
	if (Tree<Whatever>::debug)
		cerr << TREE << tree_count << COMPARE << 
		(const char *) element << AND << 
		(const char *) data << "]\n";

	/* check for duplicate */
	if (element == data) {
		data = element; // overwrite the data
		return TRUE;
	}

	/* check to go right */
	else if (element > data) {
		/* go right */
		if (!right) {

			/* debug messages */
			if (Tree<Whatever>::debug)
				cerr << TREE << 
				tree_count << INSERT << 
				(const char *) element << "]\n";
			
			/* insert the node */
			right = new TNode<Whatever>(element, *this);

			/* call SHB on the child */
			right -> SetHeightAndBalance(right);

			/* call SHB on this Node */
			this -> SetHeightAndBalance(PointerInParent);

			/* debug messages */
			if (Tree<Whatever>::debug) 
				cerr << TREE << 
				tree_count << UPDATE << 
				(const char *) data << "]\n";

			return TRUE;
		}

		/* insert somewhere down the right */
		else
			right -> Insert(element, right);
	}

	/* check to go left */
	else {
		/* go left */
		if (!left) {

			/* debug messages */
			if (Tree<Whatever>::debug)
				cerr << TREE << 
				tree_count << INSERT << 
				(const char *) data << "]\n";
			
			/* insert the node */
			left = new TNode<Whatever>(element, *this);

			/* call SHB on the child */
			left -> SetHeightAndBalance(left);

			/* call SHB on this Node */
			this -> SetHeightAndBalance(PointerInParent);

			/* debug messages */
			if (Tree<Whatever>::debug) 
				cerr << TREE << 
				tree_count << UPDATE << 
				(const char *) element << "]\n";

			return TRUE;
		}

		/* insert somewhere down the left */
		else
			left -> Insert(element, left);
	}

	/* call SHB */
	this -> SetHeightAndBalance(PointerInParent);

	/* debug messages */
	if (Tree<Whatever>::debug) 
		cerr << TREE << 
		tree_count << UPDATE << 
		(const char *) data << "]\n";

	return TRUE;
}

template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Insert
% File :         Tree.c
% 
% Description :  This function inserts a TNode into the Tree in the root, 
		it delegates all other nodes to TNode's Insert
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% const Whatever & element            the data to be stored in the TNode.
% 
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) {

        /* check for first item insertion */
	if (!root) {
		root = new TNode<Whatever>(element, *this); // insert at root

		/* debug messages */
		if (debug) 
			cerr << TREE << tree_count << INSERT <<
			(const char *) element << "]\n";
	}

	/* insert somewhere down the tree */
	else
		root -> Insert(element, root);     
	
	return TRUE;
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Lookup
% File :         Tree.c
% 
% Description :  Searches for an element in the Tree
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% Whatever & element            the data to be stored in the TNode.
% 
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {
	TNode<Whatever> * working = root;	// the current TNode

	/* check if the root is null */
	if (!root) {
		return FALSE;
	}
		
	/* for nodes other than the first one */
	else while (true) {
		
		/* debug messages */
		if (debug)
			cerr << TREE << tree_count << COMPARE <<
			(const char *) element << AND <<
			(const char *) working->data << "]\n";

		/* check for the same item */
		if (element == working->data) {
			element = working->data;  // store the data
			return TRUE;
		}
			
		/* checks to the right */
		else if (element > working->data) {

			/* return null if there's no more item */
			if (!(working->right))
				return FALSE;
					
			/* go right */
			else
				working = working->right;
		}
		
		/* check to go left */
		else {
			
			/* return null if there's no more item */
			if (!(working->left))
				return FALSE;

			/* go left  */
			else
				working = working->left;
		}
	}
}

/***************************************************************************
% Routine Name : Tree<Whatever> :: Remove
% File :         Tree.c
% 
% Description :  Removes an element from the Tree. Delegates to TNode's Remove 
		when Tree is not empty. Returns true or false indicating 
		success of removal.
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% Whatever & element            the data to be stored in the TNode.
% 
***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {
	TNode<Whatever> temp(element, *this);

	/* check if the root is null */
	if (!root)
		return FALSE;

	/* delegate to TNode's remove */
	else {
		return root -> Remove(temp, root);
	}
}
                                
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
% 
% Description :  Guarantee initialization of occupancy and root. It also 
%                initializes the tree_count using a static counter.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (void): occupancy (0), root (NULL) 

{                
	static long counter; 
	tree_count = ++counter;

	/* debug messages */
	if (debug) 
		cerr << TREE << tree_count << ALLOCATE;
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/
{
        /* delete all the nodes */
	if (root)
		root->delete_AllTNodes();

	/* debug messages */
	if (debug) 
		cerr << TREE << tree_count << DEALLOCATE;
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
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
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        return (root) ? root->Write_AllTNodes (stream) : stream;
}
