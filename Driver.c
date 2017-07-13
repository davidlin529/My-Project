/******************************************************************************
							John David Lin
							CSE 12, Spring 2017
							May 9, 2017
							cs12xkz
				Assignment six
File Name:	Driver.c
Description:	This program creates a new instance of UCSDStudent object and
		initializes all its variables. It then acts as the user 
		interface that takes input from user.
*******************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <iostream>
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

Description:            This constructor function  initializes a new 
			UCSDStudnet object. It initializes the data 
			fields, and overloads the operator. 

Input:                  nm: the student name
			sn: the student number
			student: another object of UCSDStudent
Result:                 A new UCSDStudent is initialized.
--------------------------------------------------------------------------*/
class UCSDStudent : public Base {
	char * name;		// name of variable 
	long studentnum;	// value of interest
public:
	UCSDStudent (char * nm, long sn) :
		name (strdup (nm)), studentnum (sn) {}
	~UCSDStudent (void) {
		free(name);
	}

	/* Overloaded constructor that takes in an object */
	UCSDStudent (UCSDStudent & student) {
		name = strdup(student.name);	// set the name field
		studentnum = student.studentnum; //set the studentnum field
	}
	
	/* overloaded operator, getter for name */
	operator char * (void) {
		return name;
	}

	/* overloaded operator, getter for hashCode */
	operator long (void);


	/* overloaded operator, compare 2 objects */
	long operator == (Base & base) {
		return ! strcmp (name, base);
	}

	ostream & Write (ostream & stream) {
		return stream << "name: " << name
			<< " Studentnum: " << studentnum;
	}

};

/* the operator function */
UCSDStudent :: operator long(void) {
	long sum = 0;	/* ACSII sum */
	
	/* add the ACSII sum */
	for (long index = 0; name[index]; index++)
		sum += name[index];

	return sum;
}

int main (int argc, char * const * argv) {
	SymTab ST (5);
	char buffer[128];
	char command;
	long number;
	char option;

	/* initialize debug variable */
	HashTable::Set_Debug(0);

	 /* check command line options for debug display */
	 while ((option = getopt (argc, argv, "x")) != EOF) {

		 switch (option) {
			 case 'x': HashTable::Set_Debug(1);
				 break;
		 	}
	} 

	ST.Write (cout << "Initial Symbol Table:\n");
	while (cin) {
		command = NULL;	// reset command each time in loop
		cout << "Please enter a command:  ((i)nsert, (l)ookup, (w)rite):  ";
		cin >> command;

		switch (command) {
		case 'i':
			cout << "Please enter UCSD student name to insert:  ";
			cin >> buffer;	// formatted input

			cout << "Please enter UCSD student number:  ";
			cin >> number;

			// create student and place in symbol table
			if(!ST.Insert (new UCSDStudent (buffer, number))){

				ST.Write (cout << "\nFinal Symbol Table:\n");
				exit (0);
			}
			break;

		case 'l': {
			Base * found;	// whether found or not

			cout << "Please enter UCSD student name to lookup:  ";
			cin >> buffer;	// formatted input

			UCSDStudent stu (buffer, 0);
			found = ST.Lookup (& stu);
			
			if (found)
				found->Write (cout << "Student found!!!\n") << "\n";
			else
				cout << "Student " << buffer << " not there!\n";
			}
			break;

		case 'w':
			ST.Write (cout << "The Symbol Table contains:\n");
		}
	}
	ST.Write (cout << "\nFinal Symbol Table:\n");
}

