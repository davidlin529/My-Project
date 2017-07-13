/*----------------------------------------------------------------------------
							John David Lin
							CSE 12, Spring 2017
							May 15, 2017
							cs12xkz
				Assignment Seven
File Name:	Tree.java
Description:	This program creates a tree and allows operations of insert,
		lookup, and remove on TNodes on the tree. 
*******************************************************************************/

public class Tree<Whatever extends Base> extends Base {

        /* data fields */
        private TNode root;
        private long occupancy; 
        private String treeName;

        /* debug flag */
        private static boolean debug;

        /* debug messages */
        private static final String ALLOCATE = " - Allocating]\n";
        private static final String AND = " and ";
        private static final String COMPARE = " - Comparing ";
        private static final String INSERT = " - Inserting ";
        private static final String TREE = "[Tree ";			


        public Tree (String name) {
		treeName = name;	// the name of the tree
		occupancy = 0;		// number of items it holds
		root = null;		// pointer to root

		/* debug messages */
		if (debug) 
			System.err.println(TREE + treeName + ALLOCATE);
        }

	/**
	 * turns debug off
	 * */
        public static void debugOff () {
		debug = false;
        }

	/**
	 * turns debug on
	 * */
        public static void debugOn () {
		debug = true;
        }

        /**
         * Returns the tree's name
         * @return name of the tree
         */
        public String getName() {
                return treeName;
        }

	/**
	 * Insert an element into the tree
	 * @param element the element to insert
	 */ 
        public boolean insert (Whatever element) {
		TNode working = root;	// the current TNode

		/* check for first item insertion */
		if (root == null) {
			working = root = new TNode(element); //insert at root

			/* debug messages */
			if (debug) 
				System.err.println(TREE + treeName + INSERT + 
				element.getName() + "]");
		}

		/* insert somewhere down the tree */
		else while (true) {

			/* debug messages */
			if (debug)
				System.err.println(TREE + treeName + COMPARE + 
				element.getName() + AND + 
				working.data.getName() + "]");

			/* check for duplicate */
			if (element.equals(working.data)) {
				working.data = element;
				if (working.hasBeenDeleted == true) {
					working.hasBeenDeleted = false;
					occupancy ++;
				}
				break;
			}

			/* check to go right */
			if (element.isGreaterThan(working.data)) {
				/* go right */
				if (working.right == null) {

					/* debug messages */
					if (debug)
						System.err.println(TREE + 
						treeName + INSERT + 
						element.getName() + "]");

					/* insert the node and update the 
					 * parent and working pointers 
					 */
					working.right = new TNode(element);
					working.right.parent = working;
					working = working.right;
					break;
				}

				/* insert somewhere down the right */
				else 
					working = working.right;
			}
			
			/* check to go left */
			else {
				/* go left */
				if (working.left == null) {

					/* debug messages */
					if (debug)
						System.err.println(TREE + 
						treeName + INSERT + 
						element.getName() + "]");
					
					/* insert the node and update the 
					 * parent and working pointers 
					 */
					working.left = new TNode(element);
					working.left.parent = working;
					working = working.left;
					break;
				}
				
				/* inset somewhere down the let */
				else
					working = working.left;
			}
		}

		/* travel back to the parent nodes to set 
		 * heights and balances
		 */
		while (working.parent != null) {
			working = working.parent;  // go up to the parent
			long leftHeight = -1;	// height of the left child
			long rightHeight = -1;	// height of the right child

			/* set leftHeight */
			if (working.left != null)
				leftHeight = working.left.height;

			/* set rightHeight */
			if (working.right != null) 
				rightHeight = working.right.height;

			/* height = tallest child + 1 */
			if (leftHeight > rightHeight)
				working.height = leftHeight + 1;
			else
				working.height = rightHeight + 1;

			/* set balance */
			working.balance = leftHeight - rightHeight;
		}
		
		return true;
        }

	/**
	 * Look up at item from the tree
	 * @param element the element to look up
	 */
        public Whatever lookup (Whatever element) {
		TNode working = root;	// the current TNode

		/* check if the root is null */
		if (root == null) {
			return null;
		}
		
		/* for nodes other than the first one */
		else while (true) {
			
			/* debug messages */
			if (debug)
				System.err.println(TREE + treeName + COMPARE + 
				element.getName() + AND + 
				working.data.getName() + "]");

			/* check for the same item */
			if (element.equals(working.data)) {
				
				/* if the item has't been deleted */
				if (!working.hasBeenDeleted)
					return working.data;

				/* item has been deleted */
				else 
					return null;
			}
			
			/* checks to the right */
			if (element.isGreaterThan(working.data)) {

				/* return null if there's no more item */
				if (working.right == null) 
					return null;
					
				/* go right */
				else
					working = working.right;
			}
			
			/* check to go left */
			else {

				/* return null if there's no more item */
				if (working.left == null) 
					return null;

				/* go left  */
				else
					working = working.left;
			}
		}
        }

	/**
	 * Remove an item fron the tree
	 * @param element the element to remove from the tree
	 */
        public Whatever remove (Whatever element) {
                TNode working = root;	// the current TNode

		/* check if the root is null */
		if (root == null) {
			return null;
		}

		/* for nodes other than the first one */
		else while (true) {
			
			/* debug messages */
			if (debug)
				System.err.println(TREE + treeName + COMPARE + 
				element.getName() + AND + 
				working.data.getName() + "]");

			/* check for the same item */
			if (element.equals(working.data)) {
				
				/* if the item hasn't been deleted */
				if (!working.hasBeenDeleted) {
					working.hasBeenDeleted = true;
					occupancy --;
					return working.data;
				}
				
				/* item has been deleted */
				else 
					return null;
			}
			
			/* checks to the right */
			if (element.isGreaterThan(working.data)) {

				/* return null if there's no more item */
				if (working.right == null) 
					return null;
				
				/* go right */
				else
					working = working.right;
			}

			/* check to go left */
			else {
				
				/* return null if there's no more item */
				if (working.left == null) 
					return null;

				/* go left */
				else
					working = working.left;
			}
		}
        }

        /**
         * Creates a string representation of this tree. This method first
         * adds the general information of this tree, then calls the
         * recursive TNode function to add all nodes to the return string 
         *
         * @return  String representation of this tree 
         */
        public String toString () {
                String string = "Tree " + treeName + ":\noccupancy is ";
                string += occupancy + " elements.";

                if(root != null)
                        string += root.writeAllTNodes();

                return string;
        }

        private class TNode {

                public Whatever data;
                public TNode left, right, parent;
                public boolean hasBeenDeleted;

                /* left child's height - right child's height */
                public long balance;
                /* 1 + height of tallest child, or 0 for leaf */
                public long height;

                public TNode (Whatever element) {
			data = element;			// stores the element
			left = right = parent = null; //set all pointers to null
			hasBeenDeleted = false;		// has not been deleted
			balance = 0;			// set balance to 0
			height = 0;			// set height to 0
			occupancy ++;			// one more TNode
                }

                /**
                 * Creates a string representation of this node. Information
                 * to be printed includes this node's height, its balance,
                 * and the data its storing.
                 *
                 * @return  String representation of this node 
                 */

                public String toString () {
                        return "at height:  " + height + "  with balance: " +
                                balance + "  " + data;
                }

                /**
                 * Writes all TNodes to the String representation field. 
                 * This recursive method performs an in-order
                 * traversal of the entire tree to print all nodes in
                 * sorted order, as determined by the keys stored in each
                 * node. To print itself, the current node will append to
                 * tree's String field.
                 */
                public String writeAllTNodes () {
                        String string = "";
                        if (left != null)
                                string += left.writeAllTNodes ();
                        if (!hasBeenDeleted) 
                                string += "\n" + this;          
                        if (right != null)
                                string += right.writeAllTNodes ();

                        return string;
                }
        }
}
