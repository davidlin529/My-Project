/*************************************************************************
 *							John David Lin
 *							CSE 12, Spring 2017
 *							4/11/2017
 *							cs12xkz
 *				Assignment Two
 * File Name:	hw2.java
 * Description:	This program tests functions to display output strings and
 *		numbers
 *************************************************************************/


/**
 * The hw2 class is a direct port of hw2.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

class MyLibCharacter {
        private Character character;

        public MyLibCharacter (int ch) {
                character = new Character ( (char) ch );
        }

        public char charValue () {
                return character.charValue ();
        }

        public String toString () {
                return "" + character;
        }
}

public class hw2 {
	private static final int ASCII_ZERO = 48;

	private static final int CR = 13;		// Carriage Return
	private static final int MAXLENGTH = 80;	// Max string length

	private static final int EOF = -1;		// process End Of File

	private static final long COUNT = 16;		// # of hex digits

	private static final long DECIMAL = 10;		// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// for ASCII conversion
	     new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

	private static final String DEBUG_GETALINE = 
		"[*DEBUG:  The length of the string just entered is ";

	private static final String DIGIT_STRING = "digit ";
	private static final String REENTER_NUMBER ="\nPlease reenter number: ";
	private static final String OUT_OF_RANGE = " out of range!!!\n";
	private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
	private static final String DEBUG_WRITELINE =
		"\n[*DEBUG:  The length of the string displayed is ";

	private static Stack<MyLibCharacter> InStream =
		new Stack<MyLibCharacter>();

	private static boolean debug_on = false;
	private static long hexCounter = 0; // counter for the number hex digits

	/**
	 * Function name: baseout
	 * Description:	Takes in a positive number and displays in a given base.
	 *
	 * @param number Numeric value to be displayed.
	 * @param base Base to used to display number.
	 * @param stream Where to display, likely System.out or System.err.
	 *
	 * Result:  when the base is HEX, the printed number is in hexadecimal
	 *          format. When the base is DECIMAL, the printed number is in
	 *          decimal format.
	 *          No return value
	 */
	private static void baseout (long number, long base, PrintStream stream) {
		char digit;	/* each digit of a number to add to myArray */
		int index = 0;	/* to track the number in myArray */
		char [] myArray = new char[1024];/*stores each digit of number*/

		/*
		 * to divide the number by the base to get a digit repeatedly,
		 * and then store each digit in myArray, until the number is 0,
		 * meaning no more digits can be extracted.
		 */
		while (true) {
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
			int count = ((int)HEX - index - 1);
			while (count > 0){
				fputc('0', stream);
				count --;
			}
		}

		/* pass each digit stored in myArray to be printed */
		while (index >= 0) {
			fputc(myArray[index], stream);
			index--;
		}
	}

	/**
	 * Function name: clrbuf
	 * Description: clears the buffer by calling fgetc repeatedly
	 * @param character
	 *
	 * Result: if the parameter is null, the buffer is already empty,
	 * 			otherwise, clear the buffer.
	 */
	public static void clrbuf (int character) {
		/*
		clears the buffer if the parameter isn't null
		 */
		if (character != '\n') {
			/*
			clear the buffer by calling fgetc repeatedly until the null character
			 */
			while (fgetc(System.in) != '\n') {
			}
		}
	}


	/**
	 * Function name: decin
	 * Purpose: takes in the input and convert it to integer
	 * Description: Reads a decimal number from stdin and converts it from a
	 * 				sequence of ASCII characters into a decimal integer. This
	 * 				converted value is returned. When an overflow occurs or
	 * 				non-digit characters are entered, the user is prompted
	 * 				to reenter the number using the digiterror function
	 * @return sum - the number format of the input
	 * Results: Returns the number when fgetc gets a newline
	 * 			The program terminates when ctrl-D is pressed
	 * 			Displays error message when characters are out of range
	 * 			Displays error message when the number causes overflow
	 */
	public static long decin() {
		int digit;		// stores a digit from the input
		int sum = 0; 	// keep track of the integer
		int lastSum = 0;// stores the previous sum to check for overflow
		/*
		 * Repeatedly call fgetc to take characters and convert them to a number
		 */
		while (true) {
			digit = fgetc(System.in);
			/*
			 * Break when taken in a newline character
			 */
			if (digit == '\n') {
				break; // exit the loop
			}
			/*
			 * Exit the program when ctrl-D is pressed
			 */
			if (digit == EOF) {
				return EOF; // exit the program
			}
			/*
			 * Displays error message when characters are out of range
			 * and reset all variables to 0
			 */
			if ((digit > '9') || (digit < '0')) {
				digiterror(digit, OUT_OF_RANGE);
				sum = 0;
				lastSum = 0;
				digit = ASCII_ZERO;
			}

			sum = (sum * 10) +  (digit - ASCII_ZERO); // the conversion algorithm

			/*
			 * if the sum/10 doesn't equal to previous sum, there's overflow,
			 * and display error message
			 */
			if (lastSum != (sum / 10)) {
				digiterror(digit, CAUSED_OVERFLOW);
				sum = 0;
			}

			lastSum = sum; //store sum in previous sum
		}
		return sum;
	}

	/**
	 * Function Name: decout
	 * Purpose: Takes in a positive number and displays it in decimal.
	 * Description: Goal is achieved via delegating to the baseout function.
	 *
	 * @param number positive numeric value to be displayed.
	 * @param stream Where to display, likely System.out or System.err.
	 * Results:	Number in base 10 is displayed.
	 * No return value
	 */
	public static void decout (long number, PrintStream stream) {
		writeline("", stream);
		/* prints the nunber in decimal format */
		baseout(number, DECIMAL, stream);
	}


	/**--------------------------------------------------------------------------
	Function Name:          digiterror
	Purpose:                This function handles erroneous user input.
	Description:            This function  displays and error message to the user,
				and asks for fresh input.
	Input:                  character:  The character that began the problem.
				message:  The message to display to the user.
	Result:                 The message is displayed to the user.
				The result in progress needs to be set to 0 in
				decin after the call to digiterror.
	--------------------------------------------------------------------------*/
	public static void digiterror (int character, String message) {

		/* handle error */
		clrbuf (character);

		/* output error message */
		writeline (DIGIT_STRING, System.err);
		fputc ( (char)character, System.err);
		writeline (message, System.err);

		writeline (REENTER_NUMBER, System.err);
	}


	/**
	 * Function name: getaline
	 * Purpose: get a line of string from the input
	 * Description: reads characters from stdin or System.in and stores them
	 * 				into the message array. The number of characters stored
	 * 				into message must not exceed maxlength (including NULL character).
	 * 				Any extra characters entered to stdin are discarded using clrbuf.
	 * @param message an array to hold the characters entered
	 * @param maxlength the size of the message array
	 * @return length of the message
	 *
	 * Results: Returns the length of the message when fgetc gets a newline
	 * 			The program terminates when ctrl-D is pressed
	 * 			If the message length exceeds maxlength, call clrbuf to clear buffer
	 */
	public static long getaline(char message[], int maxlength) {
		int index = 0; 		// keeps track of the message
		int character;		// stores individual characters from message

		/*
		 * keeps popping a charcter from the input until reacing a newline
		 */
		while (true) {
			character = fgetc(System.in);
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

			message[index] = (char)character; // stores each character in the array
			index ++;

			/*
			 * update maxlength to match the array's length
			 */
			if (message.length < maxlength) {
				maxlength = message.length;
			}

			/*
			 * if the message's size exceeds mexlength, call clrbuf and exit the loop
			 */
			if (index == maxlength - 1) {
				message[index] = '\0';  // append a null character at the end
				clrbuf(character);		// clear buffer

				/*
				 * prints debug messages if debug is on
				 */
				if (debug_on) {
					System.err.println(DEBUG_GETALINE + index + "]");
				}
				return index;
			}

		}

		/*
		 * prints debug messages if debug is on
		 */
		if (debug_on) {
			System.err.println(DEBUG_GETALINE + index + "]");
		}

		message[++index] = '\0';		// append a null character at the end
		return index - 1;
	}

	/**
	 * Function Name: hexout
	 * Purpose: Takes in a positive number and displays it in hex.
	 * Description: Goal is achieved via delegating to the baseout
	 * funciton
	 *
	 * @param number A positive numeric value to be displayed in hex.
	 * @param stream Where to display, likely System.out or System.err.
	 *
	 * Results:     Number in base 16 is displayed.
	 *              No return value.
	 */
	public static void hexout (long number, PrintStream stream) {
		// Output "0x" for hexidecimal.
		writeline ("0x", stream);
		baseout (number, HEX, stream);
	}

	/**
	 * Returns a character from the input stream.
	 *
	 * @return  <code>char</code>
	 */
	public static int fgetc(InputStream stream) {

		char ToRet = '\0';

		// Check our local input stream first.
		//   If it's empty read from System.in
		if (InStream.isEmpty ()) {

			try {
				// Java likes giving the user the
				// CR character too. Dumb, so just
				// ignore it and read the next character
				// which should be the '\n'.
				ToRet = (char) stream.read ();
				if (ToRet == CR)
					ToRet = (char) stream.read ();

				// check for EOF
				if ((int) ToRet == 0xFFFF)
					return EOF;
			}

			// Catch any errors in IO.
			catch (EOFException eof) {

				// Throw EOF back to caller to handle
				return EOF;
			}

			catch (IOException ioe) {

				writeline ("Unexpected IO Exception caught!\n",
						System.out);
				writeline (ioe.toString (), System.out);
			}

		}

		// Else just pop it from the InStream.
		else
			ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
		return ToRet;
	}

	/**
	 * Displays a single character.
	 *
	 * @param CharToDisp Character to display.
	 */
	public static void fputc(char CharToDisp, PrintStream stream) {

		// Print a single character.
		stream.print (CharToDisp);

		// Flush the system.out buffer, now.
		stream.flush ();
	}

	/**
	 * Function Name: newline
	 * Purpose: Prints out a newline character.
	 * Description: Prints a newline via calling fputc
	 *
	 * @param stream Where to display, likely System.out or System.err.
	 *
	 * Results:	A newline is displayed.
	 *		No return value.
	 */
	public static void newline (PrintStream stream) {
		fputc('\n', stream); // prints a newline
	}

	/**
	 * Function Name: writeline
	 * Purpose: Prints out a string.
	 * Description: Passes each character to the parameter FILE stream
	 *
	 * @param message A string to print out.
	 * @param stream Where to display, likely System.out or System.err.
	 * @return     <code>int</code> The length of the string.
	 * Results:	The message is displayed, stderr before stdout
	 */
	public static long writeline (String message, PrintStream stream) {
		int index = 0;	// to track the message

		/* Passes each character of the message to fputc until
		 * the end of the message
		 */
		while ((index < message.length()) && (message.charAt(index) != '\0')){
			fputc(message.charAt(index), stream);
			index ++;
		}

		/*
		 * print out the debug message if debug is on
		 */
		if (debug_on) {
			System.err.println(DEBUG_WRITELINE + index + "]");
		}

		return index + 1;	// returns the length of the message
	}

	/**
	* Places back a character into the input stream buffer.
	*
	* @param ToPutBack A character to putback into the input buffer stream.
	*/
	public static void ungetc (int ToPutBack) {

		// Push the char back on our local input stream buffer.
		InStream.push (new MyLibCharacter (ToPutBack));
	}

	public static void main( String[] args ) {

		char buffer[] = new char[MAXLENGTH];       /* to hold string */

		long number;                  /* to hold number entered */
		long strlen;                  /* length of string */
		long base;		      /* to hold base entered */

		/* initialize debug states */
		debug_on = false;

		/* check command line options for debug display */
		for (int index = 0; index < args.length; ++index) {
			if (args[index].equals("-x"))
				debug_on = true;
		} 

		/* infinite loop until user enters ^D */
		while (true) {
			writeline ("\nPlease enter a string:  ", System.out);

			strlen = getaline (buffer, MAXLENGTH);
			newline (System.out);

			/* check for end of input */
			if ( EOF == strlen )
				break;

			writeline ("The string is:  ", System.out);
			writeline ( new String(buffer), System.out);

			writeline ("\nIts length is ", System.out);
			decout (strlen, System.out);
			newline (System.out);

			writeline ("\nPlease enter a decimal number:  ", System.out);
			if ((number = decin ()) == EOF)
				break;

			writeline ("\nPlease enter a decimal base:  ", System.out);
			if ((base = decin ()) == EOF)
				break;

			/* correct bases that are out of range */
			if (base < 2)
				base = 2;
			else if (base > 36)
				base = 36;

			newline (System.out);

			writeline ("Number entered in base ", System.out);
			decout (base, System.out);
			writeline (" is: ", System.out);
			baseout (number, base, System.out);

			writeline ("\nAnd in decimal is:  ", System.out);
			decout (number, System.out);

			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number, System.out);

			writeline ("\nNumber entered multiplied by 8 is:  ", System.out);
			decout (number << 3, System.out);
			writeline ("\nAnd in hexidecimal is:  ", System.out);
			hexout (number << 3, System.out);

			newline (System.out);
		}
	}
}
