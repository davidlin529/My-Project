import java.io.EOFException;

/*----------------------------------------------------------------------------
							John David Lin
							CSE 12, Spring 2017
							May 9, 2017
							cs12xkz
				Assignment six
File Name:	Driver.java
Description:	This program creates a new instance of UCSDStudent object and
		initializes all its variables. It then acts as the user 
		interface that takes input from user.
*******************************************************************************/
 
class UCSDStudent extends Base {
        private String name;		// name of variable 
        private long Studentnum;	// value of interest

        public String toString () {

                return "name: " + name + " Studentnum: " + Studentnum;
        }

	/**
	 * overloaded constructor that takes in a string and a long 
	 *
	 * @param nm  name of variable
	 * @param val value of interest
	 * */
	public UCSDStudent(String nm, long val) {
		name = new String(nm);	// name of variable
		Studentnum = val;	// value of interest
	}

	/**
	 * Overloaded constructor that takes in an object 
	 *
	 * @param student another object of UCSDStudent 
	 * */
	public UCSDStudent(UCSDStudent student) {
		name = new String(student.name);    // name of variable
		Studentnum = student.Studentnum;    // value of interest
	}
	
	/**
	 * Getter for name 
	 *
	 * @return name the name of the variable
	 * */
	public String getName() {
		return name;
	}

	/**
	 * Compares this object with other obejct
	 *
	 * @param other the object to be compared
	 **/
	public boolean equals(Object other) {
		/* if they are the same */
		if (this == other)
			return true;
		
		/* if it's not a UCSDStudent type */
		if (!(other instanceof UCSDStudent))
			return false;

		/* cast it to UCSDStudent type */
		UCSDStudent otherStu = (UCSDStudent)other;

		/* compare the names */
		return name.equals(otherStu.getName());
	}

	/**
	 *  Getter for hashCode 
	 *
	 *  @return retval the hasCode
	 *
	 *  */
	public int hashCode() {
		int retval = 0;	// ASCII code
		int index = 0;	// index of the String

		/* Sum up the ASCII code */
		while (index != name.length()) {
			retval += name.charAt(index);
			index ++;
		}

		return retval;
	}

	public UCSDStudent assign(Long val) {
		UCSDStudent retval;	// return value

		// give variable its value
		Studentnum = val;
		retval = new UCSDStudent (this);
		return retval;
	}
}

public class Driver {

        public static void main (String [] args) {

                /* initialize debug states */
                HashTable.debugOff();

                /* check command line options */
                for (int index = 0; index < args.length; ++index) {
                        if (args[index].equals("-x"))
                                HashTable.debugOn();
                }

                /* The real start of the code */
                SymTab symtab = new SymTab (5);
                String buffer = null;
                char command;
                long number = 0;

                System.out.print ("Initial Symbol Table:\n" + symtab);

                while (true) {
                        command = 0;    // reset command each time in loop
                        System.out.print ("Please enter a command:  "
                                + "((i)nsert, (l)ookup, (w)rite):  ");

                        try {
                        command = MyLib.getchar ();
                        MyLib.clrbuf (command); // get rid of return

                        switch (command) {
                        case 'i':
                                System.out.print (
                                "Please enter UCSD Student name to insert:  ");
                                buffer = MyLib.getline ();// formatted input

                                System.out.print (
                                        "Please enter UCSD Student number:  ");

                                number = MyLib.decin ();
                                MyLib.clrbuf (command); // get rid of return

                                // create Student and place in symbol table
                                if(!symtab.insert (
                                        new UCSDStudent (buffer, number), 1)){

                                        System.out.println("Couldn't insert " + 
                                                           "student!!!"); 
                                }
                                break;

                        case 'l': {
                                Base found;     // whether found or not

                                System.out.print (
                                "Please enter UCSD Student name to lookup:  ");

                                buffer = MyLib.getline ();// formatted input

                                UCSDStudent stu = new UCSDStudent (buffer, 0);
                                found = symtab.lookup (stu);
                                
                                if (found != null) {
                                        System.out.println ("Student found!!!");
                                        System.out.println (found);
                                }
                                else
                                        System.out.println ("Student " + buffer
                                                + " not there!");
                                }
                                break;

                        case 'w':
                                System.out.print (
                                    "The Symbol Table contains:\n" + symtab);
                        }
                        }
                        catch (EOFException eof) {
                                break;
                        }
                }

                System.out.print ("\nFinal Symbol Table:\n" + symtab);
        }
}
