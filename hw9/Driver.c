/******************************************************************************
  John David Lin
  Lu Yu
  CSE 12, Spring 2017
  June 6, 2017
  cs12xkz
  cs12xha
  Assignment Nine
  File Name:    Driver.c
Description:    This program calls the methods and creates the commands for 
execution
 *******************************************************************************/
#include <iostream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"

using namespace std;

/* DO NOT CHANGE:  This file is used in evaluation */

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
    return stream << "name:  " << stu.name
        << " with studentnum:  " << stu.studentnum;
}

int main (int argc, char * const * argv) {
    char buffer[BUFSIZ];
    char command;
    long number;
    char option;

    /*create *is of istream type and assign it to &cin*/
    istream *is = &cin;

    /*create *os of ostream type and assign it to &cout*/
    ostream *os = &cout;
    SymTab<UCSDStudent>::Set_Debug_Off ();

    while ((option = getopt (argc, argv, "x")) != EOF) {

        switch (option) {
            case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
                      break;   
        }
    }

    SymTab<UCSDStudent> ST("Driver.datafile");
    ST.Write (cout << "Initial Symbol Table:\n" );

    while (cin) {
        command = NULL;         // reset command each time in loop
        *os << "Please enter a command ((f)ile, (i)nsert, "
            << "(l)ookup, (r)emove, (w)rite):  ";
        *is >> command;

        switch (command) {


            case 'i': {
                          *os << "Please enter UCSD student name to insert:  ";
                          *is >> buffer;  // formatted input

                          *os << "Please enter UCSD student number:  ";
                          *is >> number;

                          UCSDStudent stu (buffer, number);

                          // create student and place in symbol table
                          ST.Insert (stu);
                          break;
                      }
            case 'l': { 
                          unsigned long found;    // whether found or not

                          *os << "Please enter UCSD student name to lookup:  ";
                          *is >> buffer;  // formatted input

                          UCSDStudent stu (buffer, 0);
                          found = ST.Lookup (stu);

                          if (found)
                              cout << "Student found!!!\n" << stu << "\n";
                          else
                              cout << "student " << buffer << " not there!\n";
                          break;
                      }
            case 'r': { 
                          unsigned long removed;

                          *os << "Please enter UCSD student name to remove:  ";
                          *is >> buffer;  // formatted input

                          UCSDStudent stu (buffer, 0);
                          removed = ST.Remove(stu);

                          if (removed)
                              cout << "Student removed!!!\n" << stu << "\n";
                          else
                              cout << "student " << buffer << " not there!\n";
                          break;
                      }
            case 'f': {  

                          /*change *os and *is and prompt the user messages*/
                          *os<<"Please enter file name for command: ";

                          /*read the message*/
                          *is>>buffer;

                          /*if not inputting from keyboard*/
                          if(*is != cin){

                              /*deallocate is and os*/
                              delete is;
                              delete os;
                          }

                          /*read from file and store it in a file called 
                           * /dev/null*/
                          is = new ifstream(buffer);
                          os = new ofstream("/dev/null");
                          break;
                      }

                      /*check end of file*/
                      if(!(*is)&& *is != cin){

                          /*deallcoate is and os*/
                          delete is;
                          delete os;

                          /*assign is to cin and os to cout*/
                          is = &cin;
                          os = &cout;
                      }

                case 'w':
                        ST.Write (cout << "The Symbol Table contains:\n");
                }
        }

        ST.Write (cout << "\nFinal Symbol Table:\n");
	        
	
	if (ST.GetOperation() != 0) {
		cout << "\nCost of operations: ";
		cout << ST.GetCost();
		cout << " tree accesses";

		cout << "\nNumber of operations: ";
		cout << ST.GetOperation();

		cout << "\nAverage cost: ";
		cout << (((float)(ST.GetCost()))/(ST.GetOperation()));
		cout << " tree accesses/operation\n";
	}

	else
		cout << "\nNo cost information available.\n";


}
