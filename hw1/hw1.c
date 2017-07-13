/****************************************************************************

                                                        John David Lin
                                                        CSE12, Spring 2017
                                                        4/7/2017
                                                        cs12xkz
                                Assignment One

File Name:      hw1.c
Description:    This program tests functions to display output strings and 
                numbers. 
****************************************************************************/
#include <stdio.h>

#define COUNT ((long) (sizeof (long) << 1 )) /* number of hex digits to 
                                                display */
#define DECIMAL 10	/* to indicate base 10 */
#define HEX 16		/* to indicate base 16 */

/* Function prototypes */
void baseout (long, long, FILE *);
void decout (unsigned long, FILE *);
void hexout (unsigned long, FILE *);
void newline (FILE *);
long writeline (const char *, FILE *);

/* array used for ASCII conversion */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
	
	/* Passes each character of the message to fputc until 
	 * a NULL character 
	 */
	while (message[index] != 0) {
		fputc(message[index], stream);
		index ++;
	}

	/* retuns the length of the message */
	return index + 1;
}


int main (int argc, char *const* argv) {
	writeline ("Hello World", stdout);
	writeline ("Zdravei Sviat", stderr);
	newline(stderr);
	newline(stdout);
	decout (123, stdout);
	newline(stdout);
	decout (0, stdout);
	newline(stdout);
	hexout (0xFEEDDAD, stdout);
	newline(stdout);
	return 0;
}
