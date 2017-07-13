import java.io.*;

/*----------------------------------------------------------------------------
							John David Lin
							CSE 12, Spring 2017
							May 15, 2017
							cs12xkz
				Assignment Seven
File Name:	Driver.java
Description:	This program creates a new instance of UCSDStudent object and
		initializes all its variables. It then acts as the user 
		interface that takes input from user.
*******************************************************************************/

class UCSDStudent extends Base {
	private String name;		// name of variable
	private long studentnum;	// value of interest

	/**
	 * constructor that takes in a string and a long 
	 *
	 * @param nm  name of variable
	 * @param val value of interest
	 * */
	public UCSDStudent (String nm, long val) {
		name = new String(nm);	// name of variable
		studentnum = val;	// value of interest
	}

	/**
	 * Getter for name 
	 *
	 * @return name the name of the variable
	 * */
	public String getName () {
		return name;
	}

	/**
	 * Compares this object with other obejct
	 *
	 * @param other the object to be compared
	 **/
	public boolean equals (Object object) {
		/* if they are the same */
		if (this == object)
			return true;

		/* if it's not a UCSDStudent type */
		if (!(object instanceof UCSDStudent))
			return false;

		/* cast it to UCSDStudent type */
		UCSDStudent otherStu = (UCSDStudent) object;

		/* compare the names */
		return name.equals(otherStu.getName());
	}

	/**
	 * Checks if this object is greater than the base object
	 *
	 * @param base the base object to be compared 
	 * */
	public boolean isGreaterThan (Base base) {
		/* compare the object */
		return (name.compareTo(base.getName()) > 0) ? true : false;
	}

	public String toString () {
		return "name:  " + name + "  studentnum:  " + studentnum;
	}
}

public class Driver {
	private static final short NULL = 0;

	public static void main (String [] args) {

		/* initialize debug states */
		Tree.debugOff();

		/* check command line options */
		for (int index = 0; index < args.length; ++index) {
			if (args[index].equals("-x"))
				Tree.debugOn();
		}


		/* The real start of the code */
		SymTab<UCSDStudent> symtab = 
			new SymTab<UCSDStudent>("UCSDStudentTree");
		String buffer = null;
		char command;
		long number = 0;

		System.out.println ("Initial Symbol Table:\n" + symtab);

		while (true) {
			command = NULL; // reset command each time in loop
			System.out.print ("Please enter a command:  " + 
					"((a)llocate, (i)nsert, (l)ookup, " +
					"(r)emove, (w)rite):  ");

			try {
				command = MyLib.getchar ();
				MyLib.clrbuf (command); // get rid of return

				switch (command) {
				case 'a':
					System.out.print
						("Please enter name of new " + 
						"Tree to allocate:  ");

					buffer = MyLib.getline ();// formatted input
					symtab = new SymTab<UCSDStudent>(buffer);
					break;

				case 'i':
					System.out.print
						("Please enter UCSD student name to insert:  ");

					buffer = MyLib.getline ();// formatted input

					System.out.print
						("Please enter UCSD student number:  ");

					number = MyLib.decin ();
					MyLib.clrbuf (command); // get rid of return

					// create student and place in symbol table
					symtab.insert(new UCSDStudent (buffer, number));
					break;

				case 'l': { 
					UCSDStudent found;      // whether found or not

					System.out.print
						("Please enter UCSD student name to lookup:  ");
					buffer = MyLib.getline ();// formatted input

					UCSDStudent stu = new UCSDStudent (buffer, 0);
					found = symtab.lookup (stu);

					if (found != null) {
						System.out.println("Student found!");
						System.out.println(found);
					}
					else
						System.out.println 
							("student " + buffer
							  + " not there!");
					}
					break;

				case 'r': { 
					UCSDStudent removed; // data to be removed

					System.out.print
						("Please enter UCSD student" + 
						 " name to remove:  ");

					buffer = MyLib.getline ();

					UCSDStudent stu = new UCSDStudent (buffer, 0);

					removed = symtab.remove(stu);

					if (removed != null) {
						System.out.println("Student removed!"); 
						System.out.println(removed);
					}
					else
						System.out.println 
							("student " + buffer
							+ " not there!");
				}
				break;

				case 'w':
					System.out.println("The Symbol Table " +
							"contains:\n" + symtab);
				}
			}
			catch (EOFException eof) {
				break;
			}
		}
		System.out.println("\nFinal Symbol Table:\n" + symtab);
	}
}
