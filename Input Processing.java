/*************************************************************************
 *							John David Lin
 *							CSE 12, Spring 2017
 *							4/7/2017
 *							cs12xkz
 *				Assignment one
 * File Name:	hw1.java
 * Description:	This program tests functions to display output strings and 
 *		numbers
 *************************************************************************/

/*
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */

import java.io.*;      // System.out

public class hw1 {
	private static final long COUNT = 16; // number of hex digits to display
	private static final long DECIMAL = 10;	// to indicate base 10
	private static final long HEX = 16;		// to indicate base 16

	private static final char digits[] = 	// used for ASCII conversion
	       new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();
	private static long hexCounter = 0; // counter for the number hex digits
	private static final int BUFSIZ = 1024;

	/**
     * Function name: baseout
     * Description:	Takes in a positive number and displays in a given base.
     *
     * @param    Numeric value to be displayed.
     * @param    Base to used to display number.
     * @param    Where to display, likely System.out or System.err.
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

	/**
	 * Displays a single character
     *
	 * @param CharToDisp Character to display.
	 * @param stream Where to display, likely System.out or System.err.
	 *
	 */
	public static void fputc(char CharToDisp, PrintStream stream) {
		// Print a single character.
		stream.print (CharToDisp);

		// Flush the system.out buffer, now.
		stream.flush ();
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
		while (index < message.length()){
			fputc(message.charAt(index), stream);
			index ++;
		}
		
		return index + 1;	// returns the length of the message
	}


	public static void main( String[] args ) {
		writeline ("Zdravei Sviat", System.err);
		newline(System.err);
		writeline ("Hello World", System.out);
		newline(System.out);
		decout (123, System.out);
		newline(System.out);
		decout (0, System.out);
		newline(System.out);
		hexout (0xFEEDDAD, System.out);
		newline(System.out);
	}
}
