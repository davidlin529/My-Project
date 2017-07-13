/******************************************************************************
							John David Lin
							CSE 12, Spring 2017
							May 15, 2017
							cs12xkz
				Assignment Seven
File Name:	Driver.c
Description:	This program creates a new instance of UCSDStudent object and
		initializes all its variables. It then acts as the user 
		interface that takes input from user.
*******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string.h>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

/*--------------------------------------------------------------------------
Function Name:          UCSDStudent constructor
Purpose:                Creates a new object of UCSDStudent

Description:            This constructor function initializes a new 
			UCSDStudnet object. It initializes the data 
			fields, and overloads the operator. 

Input:                  nm: the student name
			val: the student number
Result:                 A new UCSDStudent is initialized.
--------------------------------------------------------------------------*/
class UCSDStudent : public Base {
	char * name;		// name of variable
	long studentnum;	// value of interest
public:
	/* default constructor that takes in a char and a long */
	UCSDStudent (char * nm, long val = 0) :
		name (strdup (nm)), studentnum (val) {}

	/* destructor that frees the memory */
	~UCSDStudent (void) {
		free (name);
	}

	/* overloaded operator, getter for name */
	operator const char * (void) const {
		return name;
	}

	/* overloaded operator, compare equality of 2 objects */
	long operator == (const Base & bbb) const {
		return ! strcmp (name, bbb);
	}

	/* overloaded operator, checks if this object is 
	 * greater than the base object 
	*/
	long operator > (const Base & bbb) const {
		return (strcmp (name, bbb) > 0) ? 1 : 0;
	}
	ostream & Write (ostream & stream) const {
		return stream << "name:  " << name
			<< "  studentnum:  " << studentnum;
	}
};

int main (int argc, char * const * argv) {
	char buffer[80];
	char command;
	long number;

	Tree::Set_Debug(0);

	if (argc != 1 && strcmp("-x", argv[1]) == 0) {
		Tree::Set_Debug(1);
	}
	
	SymTab * ST;
	ST = new SymTab ("UCSDStudentTree");
	ST->Write (cout << "Initial Symbol Table:\n");

	while (cin) {
		command = NULL;		// reset command each time in loop
		cout <<  "Please enter a command:  ((a)llocate, (i)nsert, " <<
			"(l)ookup, (r)emove, (w)rite):  ";
		cin >> command;

		switch (command) {

		case 'a':
			cout << "Please enter name of new Tree to allocate:  ";
			cin >> buffer; 

			delete ST;
			ST = new SymTab (buffer);

			break;

		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			ST->Insert (new UCSDStudent (buffer, number));
			break;

		case 'l': {
			const Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST->Lookup (&stu);
			
			if (found)
				found->Write (cout << "Student found!\n") << "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;
		
		case 'r': {
			Base * removed;	// data to be removed

			cout << "Please enter UCSD student name to remove:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			removed = ST->Remove (&stu);

			if (removed)
				removed->Write (cout << "Student removed!\n") << "\n";
			else
				cout << "student " << buffer << " not there!\n";
			}
			break;


		case 'w':
			ST->Write (cout << "The Symbol Table contains:\n");
		}
	}

	ST->Write (cout << "\nFinal Symbol Table:\n");
	delete ST;
}
