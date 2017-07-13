/******************************************************************************

							John David Lin
							CSE 12, Spring 2017
							April 19, 2017
							cs12xkz
				Assignment Three
File Name:	driver.c
Description:	This program reads command input from the user, then calls 
		the corresponding functions in stack.c, and then prints the 
		corresponding output.. Program terminates when user enters ^D. 

*******************************************************************************/

/******************************************************************************

Debugger questions:
1. The address returned by malloc is 0x603010
2. The address returned by new_Stack is 0x603028
3. This address is on Run-Time Stack because it is a return value
4. The value of spp is 0x7fffffffd828. spp is on Run-Time Stack because it is 
   a local variable
5. The value of *spp is 0x603028, This address is on the data section because 
   it's a global memory.
6. The value of the parameter passed to free() is 0x603010.
7. This parameter is the same address as what was returned from malloc. *

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "stack.h"

int main (int argc, char * const * argv) {

        Stack * main_Stack = 0;         /* the test stack */
        unsigned long amount;        /* numbers of items possible go on stack */
        long command;                   /* stack command entered by user */
        long item = 0;                  /* item to go on stack */
        char option;                    /* the command line option */
        long status;                    /* return status of stack functions */
        
        /* initialize debug states */
        debug_off ();

        /* check command line options for debug display */
        while ((option = getopt (argc, argv, "x")) != EOF) {
                
                switch (option) {
                        case 'x': debug_on (); break;
                }
        }

        while (1) {
                command = 0;            /* initialize command, need for loops */
                writeline ("\nPlease enter a command:", stdout);
                writeline ("\n\t(a)llocate, (d)eallocate, ", stdout);
                writeline ("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ",stdout);
                writeline ("\n\tis(f)ull, (n)um_elements, (w)rite to stdout, "
                                                                , stdout);
                writeline ("(W)rite to stderr.\n", stdout);
                writeline ("Please enter choice:  ", stdout);
                command = getchar ();
                if (command == EOF)     /* are we done? */
                        break;
                clrbuf (command);       /* get rid of extra characters :*/

                switch (command) {      /* process commands */
		
		/* allocate a new stack */
                case 'a':
			writeline ("Please enter the number of", stdout);
			writeline (" objects to be able to store: ", stdout);
			amount = decin(); /* reads an input */

			/* deallocate the current stack before allocating
			 * a new one */
			if (main_Stack != 0) 
				delete_Stack (& main_Stack);
			/* keeps track of the new stack */
			main_Stack  = new_Stack(amount);
			break;
		
		/* deallocate the stack */
		case 'd':
			delete_Stack (&main_Stack); /* call delete_Stack*/
			break;

                case 'f':               /* isfull */
                        if (isfull_Stack (main_Stack))
                                writeline ("Stack is full.\n",stdout);
                        else
                                writeline ("Stack is not full.\n", stdout);
                        break;

		/* empty the current stack */
		case 'e':	
			empty_Stack(main_Stack); /* call empty_Stack */

                case 'i':               /* isempty */
                        if (isempty_Stack (main_Stack))
                                writeline ("Stack is empty.\n", stdout);
                        else
                                writeline ("Stack is not empty.\n", stdout);
                        break;

                case 'n':               /* get_occupancy */
                        writeline ("Number of elements on the stack is:  ",
                                                                    stdout);
                        decout (get_occupancy (main_Stack));
                        newline (stdout);
                        break;

                case 'p':               /* pop */
                        status = pop (main_Stack, &item);
                        if (! status)
                                fprintf (stderr,"\nWARNING:  pop FAILED\n");
                        else {
                                writeline (
                                        "Number popped from the stack is:  ",
                                                                    stdout);
                                decout (item);
                                newline (stdout);
                        }
                        break;

                case 't':               /* top */
                        status = top (main_Stack, &item);
                        if (! status)
                                fprintf (stderr,"\nWARNING:  top FAILED\n");
                        else {
                                writeline (
                                        "Number at top of the stack is:  ",
                                                                    stdout);
                                decout (item);
                                newline (stdout);
                        }
                        break;

                case 'u':               /* push */
                        writeline (
                                "\nPlease enter a number to push to stack:  ",
                                                                    stdout);
                        item = decin ();
                        clrbuf (0);     /* get rid of extra characters */
                        status = push (main_Stack, item);
                        if (! status)
                                fprintf(stderr,"\nWARNING:  push FAILED\n");
                        break;

                case 'w':               /* write */
                        writeline ("\nThe Stack contains:\n", stdout);
                        write_Stack (main_Stack, stdout);
                        break;

                case 'W':               /* write */
                        writeline ("\nThe Stack contains:\n", stdout);
                        write_Stack (main_Stack, stderr);
                        break;
                }

                if (item == EOF) /* check if done */
                        break;
        }

        if (main_Stack)
                delete_Stack (&main_Stack);     /* deallocate stack */
        newline (stdout);
        return 0;
}

