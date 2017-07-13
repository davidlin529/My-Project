/******************************************************************************
							John David Lin
							CSE 12, Spring 2017
							May 30, 2017
							cs12xkz
				Assignment Eight
File Name:	Driver.h
Description:	This program creates a new instance of UCSDStudent object and
		initializes all its variables. 
*******************************************************************************/

#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

/*--------------------------------------------------------------------------
Class Name:             UCSDStudent 

Description:		UCSDStudent is am object to be stored in the generic
			container of Tree
--------------------------------------------------------------------------*/
class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[20];
        long studentnum;
public:
	/* default constructor that takes in a char and a long */
        UCSDStudent (char * nm, long val = 0) : studentnum (val) {
                strcpy (name, nm);
        }

	/* a copy constructor */
	UCSDStudent (const UCSDStudent & student) : 
	studentnum (student.studentnum) {
		strcpy (name, student.name);
	}

	/* overloaded operator, getter for name */
	operator const char * (void) const {
		return name;
	}

	/* overloaded operator, compare equality of 2 objects */
	long operator == (const UCSDStudent & sss) const {
		return ! strcmp (name, sss.name);
	}

	/* overloaded operator, checks if this object is 
	 * greater than the base object 
	*/
	long operator > (const UCSDStudent & sss) const {
                return (strcmp (name, sss.name) > 0) ? 1 : 0;
        }
};

#endif
