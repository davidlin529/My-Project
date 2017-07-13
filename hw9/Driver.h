/******************************************************************************
					                                   		John David Lin
                                                            Lu Yu
							                                CSE 12, Spring 2017
						                                 	May 30, 2017
						                                 	cs12xkz
                                                            cs12xha
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

	/* name is 16 bytes long */
        char name[16];
        long studentnum;
public:
	
	/* the default constructor */
	UCSDStudent() {

		/* initialize each field of name to null */
		for (unsigned int i = 0; i < sizeof(name); i++)
			name[i] = NULL;
	}
	/* default constructor that takes in a char and a long */
        UCSDStudent (char * nm, long val = 0) : studentnum (val) {
		
		/* initialize each field of name to null */
		for (unsigned int i = 0; i < sizeof(name); i++)
			name[i] = NULL;

                strcpy (name, nm);
        }

	/* a copy constructor */
	UCSDStudent (const UCSDStudent & student) : 
	studentnum (student.studentnum) {

		/* initialize each field of name to null */
		for (unsigned int i = 0; i < sizeof(name); i++)
			name[i] = NULL;

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
