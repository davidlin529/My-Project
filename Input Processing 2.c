/****************************************************************************

                                                        John David Lin
                                                        CSE 12, Spring 2017
                                                        April 12, 2017
                                                        cs12xkz
                                Assignment Two

File Name:      hw2.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/

/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define COUNT ((long) (sizeof (long) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1

#define DECIMAL 10
#define HEX 16

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout (long number, long base, FILE *stream);
void clrbuf (int);
long decin (void);
void decout (unsigned long, FILE *);
void digiterror (int, long *, const char *);
long getaline (char *, int);
void hexout (unsigned long, FILE *);
void newline (FILE *);
long writeline (const char *, FILE *);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] =  " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
        "[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
        "\n[*DEBUG:  The length of the string displayed is ";
static long debug_on = FALSE;

/*--------------------------------------------------------------------------
Function Name:		baseout
Purpose:		Prints a number in the given base to the parameter FILE
			stream
Description:		Divides the given number by the base to get each digit, 
			stores each digit in an array, and then pass each digit 
			to fputc to be printed.
Input:			umber: the number to display
			base: the base to display the number
			stream: where to display, stdout or stderr
Result:			When the base is HEX, the printed number is in 
			hexadecimal format.
			When the base is DECIMAL, the printed number is in 
			decimal format. 
			No return value
--------------------------------------------------------------------------*/
void baseout (long number, long base, FILE * stream) {
	char digit;	/* each digit of a number to be added to myArray */
	int index = 0;		/* to track the number in myArray */
	char myArray [BUFSIZ];	/* stores each digit of a number */

	/* to divide the number by the base to get a digit repeatedly,
	 * and then store each digit in myArray, until the number is 0,
	 * meaning no more digits can be extracted.
	 */
	while (1) {
		digit = digits[(int)(number % base)];
		myArray[index] = digit;
		number = number / base;

		/* break when the number is 0 */
		if (number == 0) {
			break;
		}

		index ++;
	}
	
	/* if the base is HEX, pad the remaining digits with 0's */
	if (base == HEX) {
		int count = ((int)HEX - index - 1); /* how many 0 to pad */
		while (count > 0) {
			fputc('0', stream);
			count --;
		}
	}
	
	/* pass each digit stored in myArray to be printed */
	while (index >= 0) {
		fputc(myArray[index], stream);
		index --;
	}
}



/*-------------------------------------------------------------------------- 
Function Name:          clrbuf 
Purpose:                When called, this function will clear stdin. 
Description:            This function checks to see if the incoming 
                        parameter is already '\n' indicating that stdin 
                        is already clear.  If stdin is already clear, this 
                        function does nothing.  Otherwise, this function 
                        calls "fgetc" in a loop until stdin is clear. 
Input:                  character:  the most recent character received from a 
                                previous call to fgetc. 
Result:                 stdin cleared.  Nothing returned. 
--------------------------------------------------------------------------*/  
void clrbuf (int character) {
	/*
	 * clears the buffer if the paramter isn't null
	 */
	if (character != '\n') {

		/* clear the buffer by calling fgetc repeatedly until 
		 * the null character
		 */
		while (fgetc(stdin) != '\n') {
		}
	}
}


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from from the user.
Description:            This function processes user input in a loop that ends
                        when the user enters either a valid number or EOF.
                        If EOF is entered, EOF is returned.  Otherwise each
                        character entered is checked to verify that it is
                        numeric.  Non-numeric input is identified, the user
                        is notified, reprompted, and the loop begins again.
                        Once the input is verified to be valid, a series
                        of multiplication by 10 and addition can take
                        place to convert the ASCII characters entered into
                        a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
long decin (void) {
	int digit;	/* stores a digit from the input */
	long sum = 0;	/* keep track of the integer */
	int lastSum = 0; /*stores the previous sum to check for overflow*/

	/*
	 * Repeatedly call fgetc to take charcters and convert them to 
	 * a number
	 */
	while (1) {
		digit = fgetc(stdin);

		/*
		 * Break when taken in a newline character
		 */
		if (digit == '\n') {
			break;		/* exit the loop */
		}

		/*
		 * Exit the program when ctrl-D is pressed
		 */
		if (digit == EOF) {
			return EOF;	/* exit the program */
		}

		/*
		 * Displays error messages when characters are out
		 * of range and reset all variables to 0
		 */
		if ((digit > '9') || (digit < '0')) {
			digiterror(digit, &sum, OUT_OF_RANGE);
			lastSum = 0;
			digit = ASCII_ZERO;
		}
		
		/* the conversion algorithm */
		sum = (sum * 10) +  (digit - ASCII_ZERO);

		/*
		 * if the sum/10 doesn't equal to previous sum,
		 * there's overflow, and display error messages
		 */
		if (lastSum != (sum / 10)) {
			digiterror(digit, &sum, CAUSED_OVERFLOW);
		}

		lastSum = sum;		/* stores sum in previous sum */
	}
	return sum;
}


/*--------------------------------------------------------------------------
Function Name:         decout
Purpose:               Prints a number in base 10 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:		       number:	the number to display
		       stream:	where to display, likely stdout or stederr
Result:                Number in base 10 is displayed.
		       No return value. 
--------------------------------------------------------------------------*/
void decout (unsigned long number, FILE * stream) {
	writeline("", stream);
	/* prints the number in decimal format */
	baseout(number, DECIMAL, stream);
}


/*--------------------------------------------------------------------------
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
                        error message to the user, and asks for fresh
                        input.
Input:                  character:  The character that began the problem.
                        sum:  A pointer to the sum to reinitialize.
                        message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror (int character, long * sum, const char * message) {

	/* handle error */
	clrbuf (character);

	/* reset sum */
	*sum = 0;

	/* output error message */
	writeline (DIGIT_STRING, stderr);
	fputc (character, stderr);
	writeline (message, stderr);

	writeline (REENTER_NUMBER, stdout);
}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
                        characters.  Input is terminated when either the
                        maximum number of characters are entered, or
                        a newline character is detected.  If the user
                        enters more characters than the maximum, clrbuf
                        is called to remove extra characters which are
                        ignored.  Since this is routine accepts input,
                        if EOF is detected EOF is passed back to the main
                        program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
                                allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
                        EOF is returned when the user enters ^D.
                        Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
long getaline (char * message, int maxlength) {
	int index = 0;	/* keeps track of the message */
	int character;	/* stores individual characters from message */

	/*
	 * keeps popping a character from the input until reaching 
	 * a new line
	 */
	while (1) {
		character = fgetc(stdin);
		/*
		 * break if fgetc reaches a newline
		 */
		if (character == '\n') {
			break;
		}

		/*
		 * terminates the program when ctrl-D is pressed
		 */
		if (character == EOF) {
			return EOF;
		}

		/* stores each character in the array */
		message[index] = (char)character;
		index ++;

		/*
		 * update maxlength to match the array's length
		 */
		if (index == maxlength - 1) {
			/* append a null character at the end */
			message[index] = '\0';

			clrbuf(character);	/* clears buffer */

			/* 
			 * prints debug messages if debug is on
			 */
			if (debug_on) {
				fprintf(stderr, "%s %d%s\n", DEBUG_GETALINE, index, "]");
			}
			return index;
		}
	}
	/* 
	 * prints debug messages if debug is on		 
	 */
	if (debug_on) {
		fprintf(stderr, "%s %d%s\n", DEBUG_GETALINE, index, "]");
	}
		
	message[++index] = '\0';	/* append a null character at the end */
	return index - 1;
}

/*--------------------------------------------------------------------------
Function Name:         hexout
Purpose:               Prints a number in base 16 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
                       stream:  where to display, likely stdout or stderr
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void hexout (unsigned long number, FILE * stream) {

	/* Output "0x" for hexidecimal. */
	writeline ("0x", stream);
	baseout (number, HEX, stream);
}

/*--------------------------------------------------------------------------
Function Name:         newline
Purpose:               Prints a newline to the parameter FILE stream
Description:           Prints a newline via calling fputc
Input:                 stream:	where to display, likely stdout or stderr
Result:                A newline is displayed.
		       No return value.
--------------------------------------------------------------------------*/
void newline (FILE * stream) {
	/* prints a newline */
	fputc('\n', stream);
}

/*--------------------------------------------------------------------------
Function Name:         writeline
Purpose:               Prints the message to the parameter FILE stream
Description:           Passes each character of the message to fput, terminated
		       by a NULL character
Input:                 message:	message to display
		       stream:	where to display, likely stdout or stderr
Result:                The message is displayed, stderr before stdout
                       Returns the length of the string.
--------------------------------------------------------------------------*/
long writeline (const char * message, FILE * stream) {
	int index = 0;	/* to track the message */
	
	/* 
	 * Passes each character of the message to fputc until 
	 * a NULL character 
	 */
	while (message[index] != 0) {
		fputc(message[index], stream);
		index ++;
	}
		
	/*
	 * prints the debug messages if debug is on
	 */
	if (debug_on) {
		fprintf(stderr, "%s %d%s\n", DEBUG_WRITELINE, index, "]");
	}

	/* retuns the length of the message */
	return index + 1;
}

/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
                        and a number.  Computations are performed on the 
                        strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
                        in an infinite loop until EOF is detected.  Once EOF
                        is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main (int argc, char *const* argv) {
	char buffer[MAXLENGTH];      /* to hold string */
	long number;                  /* to hold number entered */
	long strlen;                  /* length of string */
	long base;		      /* to hold base entered */

	long array[10];               /* to show user where memory is allocated */
	long * ap = array;		/* to show user about addresses in memory */
	long ** app = &ap;		/* to show user about addresses in memory */
	long * apx = &array[0];	/* to show user about addresses in memory */
	char option;                 /* the command line option */

	/* initialize debug states */
	debug_on = FALSE;

	/* check command line options for debug display */
	while ((option = getopt (argc, argv, "x")) != EOF) {
		switch (option) {
			case 'x': debug_on = TRUE; break;
		}
	}

	/* infinite loop until user enters ^D */
	while (1) {
		writeline ("\nPlease enter a string:  ", stdout);
		strlen = getaline (buffer, MAXLENGTH);
		newline (stdout);

		/* check for end of input */
		if (strlen == EOF)
			break;

		writeline ("The string is:  ", stdout);
		writeline (buffer, stdout);

		writeline ("\nIts length is ", stdout);
		decout (strlen, stdout);
		newline (stdout);

		writeline ("\nPlease enter a decimal number:  ", stdout);
		if ((number = decin ()) == EOF)
			break;

		writeline ("\nPlease enter a decimal base:  ", stdout);
		if ((base = decin ()) == EOF)
			break;

		/* correct bases that are out of range */
		if (base < 2)
			base = 2;
		else if (base > 36)
			base = 36;

		newline (stdout);

		writeline ("Number entered in base ", stdout);
		decout (base, stdout);
		writeline (" is: ", stdout);
		baseout (number, base, stdout);

		writeline ("\nAnd in decimal is:  ", stdout);
		decout (number, stdout);

		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number, stdout);

		writeline ("\nNumber entered multiplied by 8 is:  ", stdout);
		decout (number << 3, stdout);
		writeline ("\nAnd in hexidecimal is:  ", stdout);
		hexout (number << 3, stdout);

		newline (stdout);
	}

	writeline ("\nThe value of ap is:  ", stdout);
	decout ((long) ap, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) ap, stdout);
	newline (stdout);

	writeline ("The value of app is:  ", stdout);
	decout ((long) app, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) app, stdout);
	newline (stdout);

	writeline ("The value of apx is:  ", stdout);
	decout ((long) apx, stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) apx, stdout);
	newline (stdout);

	writeline ("The value of ap + 1 is:  ", stdout);
	decout ((long) (ap+1), stdout);
	writeline ("\nAnd in hexidecimal is:  ", stdout);
	hexout ((long) (ap+1), stdout);
	newline (stdout);

	writeline ("The address of array[0] is:  ", stdout);
	decout ((long) &array[0], stdout);
	newline (stdout);

	writeline ("The address of array[1] is:  ", stdout);
	decout ((long) &array[1], stdout);
	newline (stdout);

	writeline ("The size of a float is:  ", stdout);
	decout (sizeof (float), stdout);
	newline (stdout);

	writeline ("The size of a double is:  ", stdout);
	decout (sizeof (double), stdout);
	newline (stdout);

	writeline ("The size of a long double is:  ", stdout);
	decout (sizeof (long double), stdout);
	newline (stdout);

	writeline ("The size of a char is:  ", stdout);
	decout (sizeof (char), stdout);
	newline (stdout);

	writeline ("The size of an int is:  ", stdout);
	decout (sizeof (int), stdout);
	newline (stdout);

	writeline ("The size of a short is:  ", stdout);
	decout (sizeof (short), stdout);
	newline (stdout);

	writeline ("The size of a short int is:  ", stdout);
	decout (sizeof (short int), stdout);
	newline (stdout);

	writeline ("The size of a long is:  ", stdout);
	decout (sizeof (long), stdout);
	newline (stdout);

	writeline ("The size of a long int is:  ", stdout);
	decout (sizeof (long int), stdout);
	newline (stdout);

	writeline ("The size of a long long is:  ", stdout);
	decout (sizeof (long long), stdout);
	newline (stdout);

	writeline ("The size of a signed is:  ", stdout);
	decout (sizeof (signed), stdout);
	newline (stdout);

	writeline ("The size of a signed char is:  ", stdout);
	decout (sizeof (signed char), stdout);
	newline (stdout);

	writeline ("The size of a signed short is:  ", stdout);
	decout (sizeof (signed short), stdout);
	newline (stdout);

	writeline ("The size of a signed short int is:  ", stdout);
	decout (sizeof (signed short int), stdout);
	newline (stdout);

	writeline ("The size of a signed int is:  ", stdout);
	decout (sizeof (signed int), stdout);
	newline (stdout);

	writeline ("The size of a signed long is:  ", stdout);
	decout (sizeof (signed long), stdout);
	newline (stdout);

	writeline ("The size of a signed long int is:  ", stdout);
	decout (sizeof (signed long int), stdout);
	newline (stdout);

	writeline ("The size of a signed long long is:  ", stdout);
	decout (sizeof (signed long long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned is:  ", stdout);
	decout (sizeof (unsigned), stdout);
	newline (stdout);

	writeline ("The size of an unsigned char is:  ", stdout);
	decout (sizeof (unsigned char), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short is:  ", stdout);
	decout (sizeof (unsigned short), stdout);
	newline (stdout);

	writeline ("The size of an unsigned short int is:  ", stdout);
	decout (sizeof (unsigned short int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned int is:  ", stdout);
	decout (sizeof (unsigned int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long is:  ", stdout);
	decout (sizeof (unsigned long), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long int is:  ", stdout);
	decout (sizeof (unsigned long int), stdout);
	newline (stdout);

	writeline ("The size of an unsigned long long is:  ", stdout);
	decout (sizeof (unsigned long long), stdout);
	newline (stdout);

	writeline ("The size of a void pointer is:  ", stdout);
	decout (sizeof (void *), stdout);
	newline (stdout);

	writeline ("The size of a character pointer is:  ", stdout);
	decout (sizeof (char *), stdout);
	newline (stdout);

	writeline ("The size of an int pointer is:  ", stdout);
	decout (sizeof (int *), stdout);
	newline (stdout);

	writeline ("The size of a long pointer is:  ", stdout);
	decout (sizeof (long *), stdout);
	newline (stdout);

	writeline ("The size of a float pointer is:  ", stdout);
	decout (sizeof (float *), stdout);
	newline (stdout);

	writeline ("The size of a double pointer is:  ", stdout);
	decout (sizeof (double *), stdout);
	newline (stdout);

	writeline ("The size of a long double pointer is:  ", stdout);
	decout (sizeof (long double *), stdout);
	newline (stdout);

	newline (stdout);

	return 0;
}
