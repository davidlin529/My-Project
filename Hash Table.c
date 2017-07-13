#include <cstdlib>
#include <string>
#include "Hash.h"

using namespace std;

static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable :: debug = 0;
int HashTable :: counter = 0;

void HashTable :: Set_Debug (int option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/
{
	if (option)
		debug = true;
	else
		debug = false;
}


HashTable :: HashTable (int sz) : size (sz),
	table_count(++counter), occupancy (0), table (new Base *[sz]), 
	probeCount (new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         hash.c
% 
% Description :  This function allocates an initializes the memory
%                associated with a hash table.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% size               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{
	/* setting each data field in the array to null */
	for (int tableIndex = 0; tableIndex < size; tableIndex ++) {
		table[tableIndex] = NULL;
	}

	/* debug messages */
	if (debug) {
		cerr << HASH << table_count << DEBUG_ALLOCATE;
	}

        /* initialize all table elements */

}	/* end: HashTable */


HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
	/* loop through the array to deallocate individual elements */
	for (int tableIndex = 0; tableIndex < size; tableIndex ++) {
		delete table[tableIndex];
	}
	
	delete [] table;	// deallocate the hash table
	delete [] probeCount; // deallocate the probeCount table
	

	/* debug messages */
	if (debug) {
		cerr << HASH << table_count << DEBUG_DEALLOCATE;
	}
}


bool HashTable :: Insert (Base * element, int initialCount)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
%		If the element can be inserted, the element is inserted
%		and true is returned.  Duplicate insertions will
%		cause the existing element to be deleted, and the duplicate
%		element to take its place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% recursiveCall      Whether or not we are entering from recursion
% <return>           true or false indicating success or failure of insertion
***************************************************************************/
{
        count = initialCount; // set count
	int recurCount;	      // the count of bumped item
	Base * recurItem;   // temporarilly sores the bumped item

	/* duplicate item insertion */
	if (Locate(element)) {
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
		if (table[index] == NULL) {
			table[index] = element;
			probeCount[index] = count;
			occupancy ++;

			/* debug messages */
			if (debug) 
				cerr << HASH << table_count << 
				INSERT << (char *) *element << "]\n";
			return true;
		}
			
		/* bump the existing item */
		else {
			/* debug messages */
			if (debug) {
			cerr << BUMP;
			}

			/* temporarilly stores the bumped item */
			recurCount = probeCount[index];
			recurItem = table[index];

			/* insert element */
			table[index] = element;
			probeCount[index] = count; 
			
			recurCount ++;	// increment count
			/* recursive call */
			Insert(recurItem, recurCount); 
			
			return true;
		}
	}

}


bool HashTable :: Locate (Base * element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
% 
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameters descriptions :
%  
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if tem found, or false if not found
**********************************************************************/
{
	int ASCII_Sum = (long) * element; // ACSII sum of the element
	int Loc = ASCII_Sum % size;	// the initial location
	int increment = (ASCII_Sum%(size-1))+1; // increment

	/* starts from the probe sequence of where was left off */
	Loc = (Loc+(count-1)*increment)%size;
	
	/* debug messages */
	if (debug) {
		cerr << HASH << table_count << DEBUG_LOCATE;
		cerr << PROCESSING << (char *) *element << "]\n";
		cerr << HASH_VAL <<  ASCII_Sum << "]\n";
	}

	/* keeps checking the probe sequence */
	while (true) {
		/* debug messages */
		if (debug) {
			cerr << TRYING << Loc << "]\n";
		}

		/* an empty spot if found */
		if (table[Loc] == NULL) {

			/* debug messages */
			if (debug) {
				cerr << HASH << table_count << 
				FOUND_SPOT;
			}

			/* set the index of where is left off */
			index = Loc;
			return false;
		}
		
		/* duplicate item is found */
		if (*table[Loc] == *element) {

			/* set the index of where is left off */
			index = Loc;
			return true;
		}
		
		/* the tableis exhausted */
		if (count == size + 1) {
			return false;
		}

		/* debug messages */
		if (debug) {
			cerr << HASH << table_count << 
			COMPARE << (char *) *element << AND << 
			(char *) *table[Loc] << "]\n";
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

Base * HashTable :: Lookup (Base * element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/
{
	/* debug messages */
	if (debug) {
		cerr << HASH << table_count << DEBUG_LOOKUP;
	}

	int tempCount = count; // stores count to be restored later
	count = 1;	// starts from the beginning

	/* the item is found */
	if (Locate(element)) {
		count = tempCount;	// restore count 
		return table[index];	// return the item 
	}
	else {
		count = tempCount;	// restore count
		return NULL;
	}
}


ostream & HashTable :: Write (ostream & stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
	stream << "Hash Table " << table_count << ":\n"
	       << "size is " << size << " elements, "
	       << "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (int index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ": ") << " with probeCount: "
			<< probeCount[index] << "\n";
	return stream;
}	/* end: Write */
