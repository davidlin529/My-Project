/******************************************************************************
							John David Lin
							CSE 12, Spring 2017
							May 30, 2017
							cs12xkz
				Assignment Eight
File Name:	Tree.h
Description:	This file defines the necessary class methods and variables 
		which will be implemented in Tree.c
*******************************************************************************/


#ifndef TREE_H
#define TREE_H

/* DO NOT ADD CODE: This file is used in evaluation
 * Changing function signatures will result in a 25% deduction.
 * adding comments is ok.
 */

#include <iostream>
using namespace std;

// debug message
static const char ALLOCATE[] = " - Allocating]\n";
static const char TREE[] = "[Tree ";

/***************************************************************************
% Class Name : TNode
% File :	Tree.c
% 
% Description : This class constains TNode constructors, destructors, 
		and its own funcitons. TNodes hold data and can be 
		inserted into a Tree.
***************************************************************************/
template <class Whatever>
struct TNode;

/*--------------------------------------------------------------------------
Class Name:             Tree

Description:		Tree is a genetic container that defines the 
			constrains and store objects
--------------------------------------------------------------------------*/
template <class Whatever>
class Tree {
        friend struct TNode<Whatever>;
        long occupancy;
        TNode<Whatever> * root;
        unsigned long tree_count;
        static int debug;
public:
        
        Tree (void);
        ~Tree (void);
        
        static void Set_Debug_On (void);
        static void Set_Debug_Off (void);
        
        unsigned long Insert (const Whatever &);
        unsigned long Lookup (Whatever &) const;
        unsigned long Remove (Whatever &);
        ostream & Write (ostream &) const;
};

#include "Tree.c"

#endif
