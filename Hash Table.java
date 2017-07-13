/*----------------------------------------------------------------------------
							John David Lin
							CSE 12, Spring 2017
							May 9, 2017
							cs12xkz
				Assignment six
File Name:	HashTable.java
Description:	This program performs 3 operations on the HashTable, 
		insert, locate, and loopup, elements on the table.
*******************************************************************************/

public class HashTable extends Base {

	private static int counter = 0;	// number of HashTables so far
        private static boolean debug;        // allocation of debug state

        // set in locate, last location checked in hash table 
        private int index = 0; 

        // set in insert/lookup, count of location in probe sequence
        private int count = 0;
        
        private int probeCount[];   // where we are in the probe sequence 
        private long occupancy;     // how many elements are in the Hash Table
        private int size;           // size of Hash Table
        private Base table[];       // the Hash Table itself ==> array of Base
        private int tableCount;     // which hash table it is

        // messages
        private static final String AND = " and ";			
        private static final String DEBUG_ALLOCATE = " - Allocated]\n";	
        private static final String DEBUG_LOCATE = " - Locate]\n";	
        private static final String DEBUG_LOOKUP = " - Lookup]\n";
        private static final String BUMP = "[Bumping To Next Location...]\n";
        private static final String COMPARE = " - Comparing ";	
        private static final String FOUND_SPOT = " - Found Empty Spot]\n"; 
        private static final String HASH = "[Hash Table ";		
	private static final String HASH_VAL = "[Hash Value Is ";
        private static final String INSERT = " - Inserting ";		
        private static final String PROCESSING = "[Processing ";	
        private static final String TRYING = "[Trying Index ";

        
        /**
	 * turns debug on 
	 **/
        public static void debugOn () {
		debug = true;
        }

	/**
	 * turns debug off
	 * */
        public static void debugOff () {
                debug = false;
        }

        /**
         * This methos allocates and initializes the memory
         * associated with a hash table.
         *
         * @param  sz   The number of elements for the table...MUST BE PRIME!!!
         */
        public HashTable (int sz) {
                probeCount = new int[sz];	// creates probeCount array
		occupancy = 0;		// initialize occupancy 
		size = sz;		// initialize size
		table = new Base[sz];	// creates table array
		counter ++;		// initialize counter
		tableCount = counter;	// set tableCount

		/* debug messages */
		if (debug) {
			System.err.println(HASH + tableCount + DEBUG_ALLOCATE);
		}
        }


        /**
         * This method will insert the element in the hash table.
         * If the element cannot be inserted, false will be returned.
         * If the element can be inserted, the element is inserted
         * and true is returned.  Duplicate insertions will
         * cause the existing element to be deleted, and the duplicate
         * element to take its place.
         *
         * @param   element       The element to insert.
         * @param   initialCount  Where to start in probe seq (recursive calls) 
         * @return  true or false indicating success or failure of insertion
         */
        public boolean insert (Base element, int initialCount) {
		count = initialCount;	// set count 
		int recurCount;   // the count of bumped item 
		Base recurItem;   // temporarilly sores the bumped item

		/* duplicate item insertion */
		if (locate(element)) {
			table[index] = element;
			probeCount[index] = count;
			return true;
		}
		
		/* if the table is exhausted */
		if (size == occupancy) {
			return false;
		}

		/* resursive call until an empty spot is found */
		else {
			/* an empty spot is found, insert the item */
			if (table[index] == null) {
				table[index] = element;
				probeCount[index] = count;
				occupancy ++;

				/* debug messages */
				if (debug) 
					System.err.println(HASH + tableCount + 
					INSERT + element.getName() + "]");
				return true;
			}
			
			/* bump the existing item */
			else {
				/* debug messages */
				if (debug) {
					System.err.println(BUMP);
				}
				
				/* temporarilly stores the bumped item */
				recurCount = probeCount[index];
				recurItem = table[index];
				
				/* insert element */
				table[index] = element;
				probeCount[index] = count; 
			
				recurCount ++;	// increment count 
				insert(recurItem, recurCount); //recursive call

				return true;
			}
		}
	}


        /**
         * This method will locate the location in the
         * table for the insert or lookup.
         *
         * @param   element  The element needing a location.
         * @return  true if item found, or false if not found
         */
        private boolean locate (Base element) {
		int ASCII_Sum = element.hashCode();  // ACSII sum of the element
		int Loc = ASCII_Sum % size;	   // the initial location
		int increment = (ASCII_Sum%(size-1))+1;  // increment 

		/* starts from the probe sequence of where was left off */
		Loc = (Loc+(count-1)*increment)%size;

		/* debug messages */
		if (debug) {
			System.err.println(HASH + tableCount + DEBUG_LOCATE);
			System.err.println(PROCESSING+ element.getName() + "]");
			System.err.println(HASH_VAL +  ASCII_Sum + "]");
		}
		
		/* keeps checking the probe sequence */
		while (true) {
			/* debug messages */
			if (debug) {
				System.err.println(TRYING + Loc + "]");
			}
			
			/* an empty spot if found */
			if (table[Loc] == null) {
				
				/* debug messages */
				if (debug) {
					System.err.println(HASH + tableCount + 
					FOUND_SPOT);
				}

				/* set the index of where is left off */
				index = Loc;
				return false;
			}
			
			/* duplicate item is found */
			if (table[Loc].equals(element)) {

				/* set the index of where is left off */
				index = Loc;
				return true;
			}
			
			/* the tableis exhausted */
			if (count == table.length + 1) {
				return false;
			}
			
			/* debug messages */
			if (debug) {
				System.err.println(HASH + tableCount + 
				COMPARE + element.getName() + AND + 
				table[Loc].getName() + "]");
			}

			/* found an item whose search count  is shorter than 
			 * this current element
			 */
			if (count > probeCount[Loc]) {
				index = Loc;
				return false;
			}

			/* move along the probe sequence */
			Loc = (Loc + increment) % size;
			count ++;
		}
        }


        /**
         * This method will lookup the element in the hash table.  If
         * found a pointer to the element is returned.  If the element
         * is not found, NULL will be returned to the user.
         *
         * @param   element  The element to look up.
         * @return  A pointer to the element if found, else NULL
         */
        public Base lookup (Base element) {
		/* debug messages */
		if (debug) {
			System.err.println(HASH + tableCount + DEBUG_LOOKUP);
		}

		int tempCount = count; // stores count to be restored later
		count = 1;	// starts from the beginning

		/* the item is found */
		if (locate(element)) {
			count = tempCount;	// restore count 
			return table[index];	// return the item
		}
		else {
			count = tempCount;	// restore count
			return null;
		}
        }


        /**
         * Creates a string representation of the hash table. The method 
         * traverses the entire table, adding elements one by one ordered
         * according to their index in the table. 
         *
         * @return  String representation of hash table
         */
        public String toString () {
                String string = "Hash Table " + tableCount + ":\n";
                string += "size is " + size + " elements, "; 
                string += "occupancy is " + occupancy + " elements.\n";

                /* go through all table elements */
                for (int index = 0; index < size; index++) {

                        if (table[index] != null) {
                                string += "at index " + index + ": ";
                                string += "" + table[index];
                                string += " with probeCount: "; 
                                string += probeCount[index] + "\n";
                        }
                }

                return string;
        }
}
