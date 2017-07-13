/******************************************************************************
                                                             John David Lin
                                                             Lu Yu
                                                             CSE 12, Spring 2017
                                                             June 6, 2017
                                                             cs12xkz
                                                             cs12xha
                Assignment Nine
File Name:    Tree.c
Description:    This program creates a tree and allows operations of insert,
        lookup, and remove on TNodes on the tree. 
*******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

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
% File :    Tree.c
% 
% Description : This class constains TNode constructors, destructors, 
        and its own funcitons. TNodes hold data and can be 
        inserted into a Tree.
***************************************************************************/

template <class Whatever>
struct  TNode {
// friends:

// data fields:
    Whatever data;
    long height;
    long balance;
    offset left;
    offset right;
    offset this_position;   // current position
// function fields:
    TNode () : height (0), balance (0), left (0), right (0),
        this_position (0) {}

    // to declare the working TNode in Tree's Remove
    TNode (Whatever & element) : data (element), height (0), balance (0),
        left (0), right (0), this_position (0) {}

    TNode (Whatever &, fstream *, long &);  // to add new node to disk
    TNode (const offset &, fstream *);  // to read node from disk

    unsigned long Insert (Whatever &, fstream *, long &, offset &);
    // optional recursive Lookup declaration would go here
    void Read (const offset &, fstream *);  // read node from disk
    unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
        long fromSHB = FALSE);
    void ReplaceAndRemoveMax (TNode<Whatever> &, fstream *, offset &);
    void SetHeightAndBalance (fstream *, offset &);
    void Write (fstream *) const;       // update node to disk

    ostream & Write_AllTNodes (ostream &, fstream *) const;
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

    /*set debug_on to be true*/
    debug_on = TRUE; 
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

    /*set debug_on to be false*/
    debug_on = FALSE; 
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
  % Whatever & element            the data to be stored in the TNode.
  % 
 ***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (Whatever & element) {

    /* if root is empty */
    if (occupancy == 0){

        /* create a TNode with the write constructor */
        TNode<Whatever>rootNode(element, fio, occupancy);

        /*set root to this_position*/
        root = rootNode.this_position;  

        return TRUE;
    }

    /* if root is not empty */
    else {

        /* create a TNode with the read constructor */
        TNode<Whatever> rootNode(root, fio);

        /* delegate to TNode's insert */
        return rootNode.Insert(element, fio, occupancy, root);
    }
}

 
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
  % name                description
  % ------------------ ------------------------------------------------------
  % targetTNode        a reference to the TNode to remove that has 2 children
  % fio            An fstream pointer, to access the disk file
  % PositionInParent    Reference to the TNode position in the parent TNode 
  %            used to get to the current TNode's offset in the 
  %            datafile.
 ***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMax (TNode<Whatever> & targetTNode,
        fstream * fio, offset & PositionInParent) {

    /* keep going to the right */
    if(right) {

        /* creates a RTS node with Read constructor */
        TNode<Whatever> rightNode(right, fio);

        /* call replace and remove max */
        rightNode.ReplaceAndRemoveMax(targetTNode, fio, right);
    }

    /* reach the predecessor */
    else {

        /*store the data*/
        targetTNode.data = data; 

        /*reset PositionInParent to be left*/
        PositionInParent = left;
    }

    /* call SHB */
    SetHeightAndBalance(fio, PositionInParent);
    return;
}


/***************************************************************************
  % Routine Name : TNode<Whatever> :: Remove
  % File :         Tree.c
  % 
  % Description : Remove an element from the binary tree.
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % elementTNode The target TNode that want to be deleted
  % fio			An fstream pointer, to access the disk file
  % occupancy		A long reference, to update occupancy
  % PositionInParent	Reference to the TNode position in the parent TNode 
  used to get to the current TNode's offset in the 
  datafile.
  % fromSHB: to indicate whether remove is already being called in SHAB
 ***************************************************************************/

template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
        fstream * fio, long & occupancy, offset & PositionInParent,
        long fromSHB) { 

    /*call IncrementOperation to increase operation*/
    Tree<Whatever>::IncrementOperation();

    /*unsigned long variable to hold the return value*/
    unsigned long status = FALSE; 

    /* check for the same item */
    if(elementTNode.data==data){

        /*check if the TNode has any child nodes*/
        if(!left && !right){

            /*store the data*/
            elementTNode.data = data; 

            /*reset the PositionInParent to NULL*/
            PositionInParent = 0;

            /*decrement occupancy*/
            occupancy --;    

            /*return TRUE indicate the successful removal*/
            return TRUE;
        }

        /* nodes with only right child */
        else if(!left){

            /*store the data*/
            elementTNode.data = data; 

            /*reset PositionInParent to be right*/
            PositionInParent = right; 

            /*decrement occupancy*/
            occupancy --;    

            /*return TRUE indicate the successful removal*/
            return TRUE;
        }

        /* nodes with only left child */
        else if(!right){

            /*store the data*/
            elementTNode.data = data;

            /*set PositionInParent to be left*/
            PositionInParent = left; 

            /*decrement occupancy*/
            occupancy --;    

            /*return TRUE indicate the successful removal*/
            return true;
        }

        /*if the two children exists, we call replaceandremovemax
         * on left to do recursion, then we return 0 indicating
         * the remove is unsuccessful*/
        else{

            /*hold the left node*/
            TNode<Whatever> leftNode(left, fio);

            /*deal with when two children exist*/
            leftNode.ReplaceAndRemoveMax(elementTNode, fio, left);

            /*store the data*/
            data = elementTNode.data;

            /*decremnet occupancy*/
            occupancy --;

            /*recalculate the height and balance*/
            SetHeightAndBalance(fio, PositionInParent);

            /*return TRUE indicate the successful removal*/
            return TRUE;
        }
    }

    /*if the elementTNode's data is larger, then we go to the right
     * child*/
    else if(elementTNode.data>data){

        /* return null if there's no more item */
        if(!right){
            return false;
        }

        /* otherwise, we call remove on right recursively to find
         * whether there is a matching TNode for us to delete*/
        else{
            TNode<Whatever> rightNode(right, fio);
            status = rightNode.Remove(elementTNode, fio, 
                    occupancy, right);
        }
    }

    /*if elementTNode's data is smaller, then we go to the left side*/
    else{

        /*if there is no left child, we return false, meaning that
         * the remove failed*/
        if(!left){
            return FALSE;
        }

        /*if right is null, meaning that we only got left child,
         * thus we call remove on left to find the matching TNode
         * recursively*/
        else {
            TNode<Whatever> leftNode(left, fio);

            status = leftNode.Remove(elementTNode, fio, 
                    occupancy, left);
        }
    }


    /*if we did not call remove from shab, then we call shab*/
    if(status && !fromSHB)
        SetHeightAndBalance(fio, PositionInParent);

    /*return the result of Remove*/
    return status;
}
 
/***************************************************************************
  % Routine Name : Tree<Whatever> :: Remove
  % File :         Tree.c
  % 
  % Description :  Removes an element from the Tree. Delegates to TNode's
  % Remove when Tree is not empty. Returns true or false indicating 
  % success of removal.
  %
  % Parameters descriptions :
  % 
  % name                    description
  % ------------------ ------------------------------------------------------
  % Whatever & element            the data to be stored in the TNode.
  % 
 ***************************************************************************/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {

    /* keep track of the return result */
    long status = 0;

    /* if root is empty */
    if (occupancy == 0) 

        /*return FALSE*/
        return FALSE;    

    /* if the root is not empty */
    else {
        /* create a root TNode with Read constructor */
        TNode<Whatever> rootNode(root, fio);

        /* create a TNode t hold element with the read constructor */
        TNode<Whatever> temp(element);

        /* delegate to TNode's insert */
        status = rootNode.Remove(temp, fio, occupancy, root, long(0));

        /*set the data*/
        element = temp.data;    

        /*Reset the root when the last element in the Tree is removed*/
        if (occupancy == 0)
            ResetRoot(); 

        /*return the result of TNode's Remove*/
        return status;
    }
}

/***************************************************************************
  % Routine Name : TNode<Whatever>::SetHeightAndBalance
  % File :         Tree.c
  % 
  % Description :  calculate the height and balance of a TNode in the Tree
  also, check the balance if it exceeds the threshold. If 
  it is, then remove and insert again
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % fstream * fio: An fstream pointer, to access the disk file
  % offset PositionInParent: Reference to the TNode position in the parent 
  % TNode used to get to the current TNode's offset in the datafile

 ***************************************************************************/

template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
        offset & PositionInParent) {

    long leftHeight = -1;    // height of the left child
    long rightHeight = -1;    // height of the right child

    /* set leftHeight */
    if(left){
        /* create a TNode with Read constructor */
        TNode<Whatever> leftNode(left, fio);
        leftHeight = leftNode.height;    // set height
    }

    /* set rightHeight */
    if(right){
        /* create a TNode with Read constructor */
        TNode<Whatever> rightNode(right, fio);
        rightHeight = rightNode.height;    // set height
    }

    /* height = tallest child + 1 */
    if(rightHeight>leftHeight){
        height = rightHeight + 1;
    }

    else{
        height = leftHeight + 1;
    }

    /* set balance */
    balance = leftHeight - rightHeight;

    /* over the threshold */
    if(abs(balance)>THRESHOLD){

        /* To be able to call insert and remove without real occupancy*/
        long fakeOccupancy = 0;

        /* a temporary copy of this TNode to hold the data*/
        TNode<Whatever> temp(*this);

        /* Remove this TNode */
        Remove(*this, fio, fakeOccupancy, PositionInParent, long(1));

        /* create a TNode that PositionInParent now points to, 
         * with Read constructor */
        TNode<Whatever> nextNode(PositionInParent, fio);

        /* reinsert the removed Tnode into the tree */
        nextNode.Insert(temp.data, fio, fakeOccupancy,PositionInParent);
    }

    /*update the TNodes*/
    Write(fio);    
}




/***************************************************************************
  % Routine Name : Tree<Whatever> :: GetCost
  % File :         Tree.c
  % 
  % Description :  Returns cost 
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % 
 ***************************************************************************/
template <class Whatever>
long Tree <Whatever> :: GetCost () {

    /*return cost*/
    return cost;    
}

/***************************************************************************
  % Routine Name : Tree<Whatever> :: GetOperation
  % File :         Tree.c
  % 
  % Description :  Returns operation
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % 
 ***************************************************************************/
template <class Whatever>
long Tree <Whatever> :: GetOperation () {

    /*return operation*/
    return operation;   
}

/***************************************************************************
  % Routine Name : Tree<Whatever> :: IncrementCost
  % File :         Tree.c
  % 
  % Description :  Increment Cost
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % 
 ***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: IncrementCost () {

    /*increment the cost*/
    cost ++;        
}

/***************************************************************************
  % Routine Name : Tree<Whatever> :: IncrementOperation
  % File :         Tree.c
  % 
  % Description :  Increment Operation
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % 
 ***************************************************************************/
template <class Whatever>
void Tree <Whatever> :: IncrementOperation () {

    /*increment the operation*/
    operation ++;   
}


/***************************************************************************
  % Routine Name : Tree <Whatever>::ResetRoot()
  % File :         Tree.c
  % 
  % Description : Reset the root when deleting the last element
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % 
 ***************************************************************************/

template <class Whatever>
void Tree <Whatever> :: ResetRoot () {

    /*seek to the end*/
    fio->seekp(0, ios::end);

    /*assign root to be the last*/
    root = fio->tellp();
}


/***************************************************************************
  % Routine Name : TNode<Whatever> :: Insert
  % File :         Tree.c
  % 
  % Description : Inserts an element into the binary tree.
  %
  % Parameters descriptions :
  % 
  % name					description
  % ------------------ ------------------------------------------------------
  % element		The data to be stored in the TNode
  % fio			An fstream pointer, to access the disk file
  % occupancy		A long reference, to update occupancy
  % PositionInParent	Reference to the TNode position in the parent TNode 
  used to get to the current TNode's offset in the 
  datafile.
 ***************************************************************************/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
        long & occupancy, offset & PositionInParent) {


    /*call IncrementOperation to increase the operation*/
    Tree<Whatever>::IncrementOperation();	

    /* check for duplicate */
    if(element == data){

        /*overwrite the data*/
        data = element;	
    }

    /*if the element is greater*/
    else if(element>data){

        /* insert at the right */
        if(!right) {

            /* create a TNode with Write constructor */
            TNode<Whatever> rightNode(element, fio, occupancy);
            right = rightNode.this_position;  // set right position
        }
    
        /* insert somewhere down the tree */
        else {
            /* create a TNode with Read constructor */
            TNode<Whatever> rightNode(right, fio);
 
            /* call Insert recursively */
            rightNode.Insert(element, fio, occupancy, right);
        }
    }
 
    /*if the element is smaller, then we go to the left side*/
    else{
        /* insert at the left */
        if(!left) {
            /* create a TNode with Write constructor */
            TNode<Whatever> leftNode(element, fio, occupancy);
            left = leftNode.this_position;    // set left position
        }
 
        /* insert somewhere down the tree */
        else {
            /* create a TNode with Read constructor */
            TNode<Whatever> leftNode(left, fio);
 
            /* call Insert recursively */
            leftNode.Insert(element, fio, occupancy, left);
        }
    }
    
    /*to set the height and balance for every TNode*/
    SetHeightAndBalance(fio, PositionInParent);
 
    /*return true if the insertion is successful*/
    return true;
 
}
        


/*********************************************************************
 * Function Name: Lookup
 * Purpose: find the matching TNode
 * Input: const Whatever: element, the element we are going to find in the tree
 * Output: the matching TNode will be returned
 * Return: the matching TNode
 *
 * ****************************************************************/

template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {

    /*call InncrementOperation to increase operation*/
    Tree<Whatever>::IncrementOperation();

    /*make a copy of root and call this TNode working*/
    TNode<Whatever> working(root,fio);

    /* check if the root is null */
    if(!root)

        /*return false if the root is null*/
        return FALSE;

    /* for nodes other than the first one */
    else while(true){

        /*if the element we are searching has the same value of 
         * working, make the replacement and return True*/
        if(element == working.data){
            element = working.data;
            return TRUE;
        }

        /*if the element we search is greater. then search to the right*/
        else if(element > working.data){

            /*if the working does not have a right,
             * then return FALSE*/
            if(!(working.right))
                return FALSE;

            /*otherwise we let working to be the right of 
             * working*/
            else {
                TNode<Whatever> rightNode(working.right, fio);
                working = rightNode;
            }
        }

        /*if the element we search is smaller. then search to the right*/
        else{

            /*if the working does not have a left,
             * then return FALSE*/
            if(!(working.left))
                return FALSE;


            /*otherwise we let working to be the left node of 
             * working*/
            else{
                TNode<Whatever> leftNode(working.left, fio);
                working = leftNode;
            }
        }
    }
}



/***************************************************************************
% Routine Name : TNode<Whatever> :: Read
% File :         Tree.c
% 
% Description : Reads a TNode which is present on the datafile into memory.
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% position		The location within the file where the TNode exists
% fio			An fstream pointer, to access the disk file
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio) {
	fio -> seekg(position);		// seek to the position

	/* read in the TNode */
	fio -> read((char*) this, sizeof(TNode<Whatever>));

	/* debug message */
	if (Tree<Whatever> :: debug_on) 
		cerr << COST_READ << (const char*)data << "]\n";

	Tree<Whatever>::IncrementCost();	// increment cost	
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: TNode (Read Constructor)
% File :         Tree.c
% 
% Description : TNode is created in memory for the purpose of reading an 
		existing TNode on disk into memory. Delegate to Read
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% position		The location within the file where the TNode exists
% fio			An fstream pointer, to access the disk file
***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio) {

	Read(position, fio);	// delegate to Read
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: TNode (Write Constructor)
% File :         Tree.c
% 
% Description : TNode is created in memory for the purpose of writing a 
		new TNode to disk. Delegate to Write

% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% element		Referring to the object to be stored in the TNode
% fio			An fstream pointer, to access the disk file
% occupancy		A long reference, to update occupancy
***************************************************************************/
template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio, long & occupancy):
            data (element), height (0), balance (0), left (0),
            right (0) {
	occupancy++;	// increment occupancy

	fio->seekp(0, ios::end);	// seek to the end 
	this_position = fio->tellp();	// set this position
	
	Write(fio); // delegate to Write
}

/***************************************************************************
% Routine Name : TNode<Whatever> :: Write
% File :         Tree.c
% 
% Description : Writes this TNode object to disk at this_position in the 
		datafile.
%
% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% fio			An fstream pointer, to access the disk file
***************************************************************************/
template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const {

	/* debug message */
	if (Tree<Whatever> :: debug_on) 
		cerr << COST_WRITE << (const char*)data << "]\n";

	fio -> seekp(this_position);	// seek to this position
	fio -> write((const char*) this, sizeof(*this));   // write the node

	Tree<Whatever>::IncrementCost();	}
                                
/***************************************************************************
% Routine Name : Tree<Whatever> :: Tree  (public)
% File :         Tree.c
% 
% Description : Allocates the tree object. Checks the datafile to see if it 
		contains Tree data. If it is empty, root and occupancy fields 
		are written to the file. If there is data in the datafile, root 
		and occupancy fields are read into memory

% Parameters descriptions :
% 
% name					description
% ------------------ ------------------------------------------------------
% fio			filestream corresponding to the datafile where the Tree 
			is to be stored on disk.
***************************************************************************/
template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) :
    fio (new fstream (datafile, ios :: out | ios :: in)) {


        /* keep track of tree count */
        static long tree_counter = 0;

        /*increment tree count*/
        tree_count = ++tree_counter;	

        /* debug messages */
        if(debug_on)
            cerr << TREE << tree_count << ALLOCATE;

        /*seek to the beginning*/
        fio->seekp(0, ios::beg);

        /*set the position of beginning*/
        offset beginning = fio->tellp();

        /*seek to the end*/
        fio->seekp(0, ios::end); 

        /*set the position of ending*/
        offset ending = fio->tellp();

        /* check for empty file */
        if(beginning == ending){

            /*initialize root to 0*/
            root = 0;	

            /* initialize occupancy to 0*/
            occupancy = 0;	

            /*seek to the beginning*/
            fio->seekp(0, ios::end);	

            /*write root*/
            fio->write((const char*)&root, sizeof(root)); 

            /* write occupancy */
            fio->write((const char*)&occupancy, sizeof(occupancy)); 
        }

        /* if the file is not empty */
        else{
            fio->seekg(0, ios::beg);	// seek to the beginning
            fio->read((char*)&root, sizeof(root)); // write root
            fio->read((char*)&occupancy, sizeof(occupancy)); // write occupancy
        }

        /*if occupancy is 0, reassign root*/
        if(occupancy==0){
            root= fio->tellg();
        }

    }

/***************************************************************************
  % Routine Name : Tree :: ~Tree  (public)
  % File :         Tree.c
  % 
  % Description :  deallocates memory associated with the Tree.  It
  %                will also delete all the memory of the elements within
  %                the table.
 ***************************************************************************/
template <class Whatever>
Tree<Whatever> :: ~Tree (void) {

    /* debug messages */
    if (debug_on) 
        cerr << TREE << tree_count << DEALLOCATE;

    /*seek to the beginning*/
    fio -> seekp(0, ios::beg);

    /*update root*/
    fio -> write ((const char*) &root, sizeof(root));	
    /* update occupancy */
    fio -> write ((const char*) &occupancy, sizeof(occupancy));

    /*deallocate fio*/
    delete fio;	
}	/* end: ~Tree */


template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
    stream << "at height:  :" << nnn.height << " with balance:  "
        << nnn.balance << "  ";
    return stream << nnn.data << "\n";
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This funtion will output the contents of the Tree table
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
        long old_cost = cost;

    stream << "Tree " << tree_count << ":\n"
        << "occupancy is " << occupancy << " elements.\n";

    fio->seekg (0, ios :: end);
    offset end = fio->tellg ();

    // check for new file
    if (root != end) {
        TNode<Whatever> readRootNode (root, fio);
        readRootNode.Write_AllTNodes (stream, fio);
    }

        // ignore cost when displaying nodes to users
        cost = old_cost;

    return stream;
}

template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const {
    if (left) {
                          
     TNode<Whatever> readLeftNode (left, fio);
        readLeftNode.Write_AllTNodes (stream, fio);
    }
    stream << *this;
    if (right) {
        TNode<Whatever> readRightNode (right, fio);
        readRightNode.Write_AllTNodes (stream, fio);
    }

    return stream;
}

                           


