/** @author Summer Ng
 *  @author John David Lin
 *  @version 3/13/2017
 */

public class EastWestController extends CarController {
    private int stuck = 0;

    public EastWestController (CoordInfo oracle){
        super(oracle);
        super.direction = EAST;
    }

    public void setDefaultDirection(){
        super.direction = EAST;
    }

    public Coord roam(Coord current) {
        Coord newLoc = new Coord((current.row + super.direction.row), (current.col + super.direction.col));

        if (oracle.coordFree(newLoc)) {
            return super.direction;
        }
        else {
            if (super.direction == EAST) {
                super.direction = WEST;
            }
            else if (super.direction == WEST) {
                super.direction = EAST;
            }
            else {
                super.direction = EAST;
            }
            return super.direction;
        }
    }

    public Coord drive(Coord current, Coord goal) {
		if (stuck == 0) {
			// R is west of C //
			if (goal.col < current.col) {
				super.direction = WEST;

				// west works //
				if (nextFree((current))) return WEST;

				// west does not work // R is north of C //
				stuck = 2;
				if (goal.row < current.row) {
					super.direction = NORTH;

					// north works // 
					if (nextFree(current)) return NORTH;

						// west, north do not work //
					else {
						super.direction = EAST;

						// east works //
						if (nextFree(current)) return EAST;

							// west, north, east do not work //
						else {
							super.direction = SOUTH;

							// south works //
							if (nextFree(current)) return SOUTH;
						}
					}
				}

				// west does not work // R is south of C //
				else if (goal.row > current.row) {
					super.direction = SOUTH;
		
					// south works //
					if (nextFree(current)) return SOUTH;

					// west, south do not work //
					else {
						super.direction = EAST;
	
						// east works //
						if (nextFree(current)) return EAST;
	
						// west, south, east do not work //
						else {
							super.direction = NORTH;
	
							// north works //
							if (nextFree(current)) return NORTH;
						}
					}
				}
	
				// west does not work // R is in the row of C //
				else {
					super.direction = EAST;
	
					// EAST works //
					if (nextFree(current)) return EAST;
	
					// EAST, WEST does not work //
					else { 
						super.direction = NORTH; 

						// north works //
						if (nextFree(current)) return NORTH; 
	
						// EAST, WEST, north do not work //
						else {
							super.direction = SOUTH;
	
							// east works //
							if (nextFree(current)) return SOUTH;
						}
					}
				}
			}

			// R is east of C //
			if (goal.col > current.col) {
				super.direction = EAST;
	
				// east works //
				if (nextFree(current)) return EAST;
	
				stuck = 2;
				// east does not work // R is north of C //
				if (goal.row < current.row) {
					super.direction = NORTH;
	
					// north works // 
					if (nextFree(current)) return NORTH;
	
					// east, north do not work //
					else {
						super.direction = WEST;
	
						// west works //
						if (nextFree(current)) return WEST;
	
						// west, north, east do not work //
						else {
							super.direction = SOUTH;
	
							// south works //
							if (nextFree(current)) return SOUTH;
						}
					}
				}

				// east does not work // R is south of C //
				else if (goal.row > current.row) {
					super.direction = SOUTH;
		
					// south works // 
					if (nextFree(current)) return SOUTH;
	
					// east, south do not work //
					else {
						super.direction = WEST;
	
						// west works //
						if (nextFree(current)) return WEST;
	
						// west, south, east do not work //
						else {
							super.direction = NORTH;
	
							// north works //
							if (nextFree(current)) return NORTH;
						}
					}
				}
	
				// east does not work // R is in the row of C //
				else {
					super.direction = WEST;
	
					// WEST works //
					if (nextFree(current)) return WEST;
	
					// east, WEST does not work //
					else { 
						super.direction = NORTH; 
	
						// north works //
						if (nextFree(current)) return NORTH; 
	
						// east, WEST, north do not work //
						else {
							super.direction = SOUTH;
	
							// SOUTH works //
							if (nextFree(current)) return SOUTH;
						}
					}
				}
			}
	
			// R is in the correct column of C //
			else {
	
				// R is north of C //
				if (goal.row < current.row ) {
					super.direction = NORTH;
	
					// north works // 
					if (nextFree(current)) return NORTH;
	
					// north does not work //
					stuck = 2;
					if (!nextFree(current)) {
						super.direction = WEST;
	
						// west works //
						if (nextFree(current)) return WEST;
	
						// west, north do not work //
						else {
							super.direction = EAST;
	
							// east works //
							if (nextFree(current)) return EAST;
	
							// west, north, east do not work //
							else {
								super.direction = SOUTH;
	
								// south works //
								if (nextFree(current)) return SOUTH;
							}
						}
					}
				}

				// R is south of C //
				if (goal.row > current.row ) {
					super.direction = SOUTH;
	
					// south works // 
					if (nextFree(current)) return SOUTH;

					// south does not work //
					stuck = 2;
					if (!nextFree(current)) {
						super.direction = WEST;
	
						// west works //
						if (nextFree(current)) return WEST;
	
						// west, south do not work //
						else {
							super.direction = EAST;
	
							// east works //
							if (nextFree(current)) return EAST;
	
							// west, south, east do not work //
							else {
								super.direction = NORTH;
	
								// north works //
								if (nextFree(current)) return NORTH;
							}
						}
					}
				}
			}
		}
		else {
			this.switchDir(current, goal);
		}
		return direction;
    }

	private boolean nextFree(Coord current) {
		Coord newLoc = new Coord ((current.row + super.direction.row), (current.col + super.direction.col));
		return oracle.coordFree(newLoc);
	}

	// aka NORTHSOUTH CONTROLLER //
    private Coord switchDir (Coord current, Coord goal) {
		stuck++;
		stuck = stuck % 2;
		// R is north of C //
			if (goal.row < current.row) {
				super.direction = NORTH;
	
				// north works //
				if (nextFree((current))) return NORTH;
	
				// north does not work // R is west of C //
				else if (goal.col < current.col) {
					super.direction = WEST;
	
					// west works // 
					if (nextFree(current)) return WEST;
	
					// west, north do not work //
					else {
						super.direction = SOUTH;
	
						// south works //
						if (nextFree(current)) return SOUTH;
	
						// west, north, south do not work //
						else {
							super.direction = EAST;
	
							// east works //
							if (nextFree(current)) return EAST;
						}
					}
				}

				// north does not work // R is east of C //
				else if (goal.col > current.col) {
					super.direction = EAST;
		
					// east works // 
					if (nextFree(current)) return EAST;

					// north, east do not work //
					else {
						super.direction = SOUTH;
	
						// south works //
						if (nextFree(current)) return SOUTH;
	
						// north, south, east do not work //
						else {
							super.direction = WEST;
	
							// north works //
							if (nextFree(current)) return WEST;
						}
					}
				}
	
				// north does not work // R is in the col of C //
				else {
					super.direction = EAST; 
	
					// east works //
					if (nextFree(current)) return EAST;
	
					// east, north does not work //
					else { 
						super.direction = WEST; 

						// west works //
						if (nextFree(current)) return WEST; 
	
						// west, east, north do not work //
						else {
							super.direction = SOUTH;
	
							// south works //
							if (nextFree(current)) return SOUTH;
						}
					}
				}
			}

			// R is south of C //
			if (goal.row > current.row) {
				super.direction = SOUTH;
	
				// south works //
				if (nextFree(current)) return SOUTH;
	
				// south does not work // R is west of C //
				else if (goal.col < current.col) {
					super.direction = WEST;
	
					// west works // 
					if (nextFree(current)) return WEST;
	
					// west, south do not work //
					else {
						super.direction = NORTH;
	
						// north works //
						if (nextFree(current)) return NORTH;
	
						// west, north, south do not work //
						else {
							super.direction = EAST;
	
							// east works //
							if (nextFree(current)) return EAST;
						}
					}
				}

				// south does not work // R is east of C //
				else if (goal.col > current.col) {
					super.direction = EAST;
		
					// east works // 
					if (nextFree(current)) return EAST;
	
					// east, south do not work //
					else {
						super.direction = NORTH;
	
						// north works //
						if (nextFree(current)) return NORTH;
	
						// north, south, east do not work //
						else {
							super.direction = WEST;
	
							// west works //
							if (nextFree(current)) return WEST;
						}
					}
				}

				// south does not work // R is in the col of C //
				else {
					super.direction = EAST; 
	
					// east works //
					if (nextFree(current)) return EAST;
	
					// east, south does not work //
					else { 
						super.direction = WEST; 
	
						// west works //
						if (nextFree(current)) return WEST; 
	
						// east, south, west do not work //
						else {
							super.direction = NORTH;
	
							// north works //
							if (nextFree(current)) return NORTH;
						}
					}
				}
			}
			// R is in the correct row of C //
			else {

				// R is west of C //
				if (goal.col < current.col) {
					super.direction = WEST;

					// west works // 
					if (nextFree(current)) return WEST;
	
					// west does not work //
					else {
						super.direction = NORTH;
	
						// north works //
						if (nextFree(current)) return NORTH;
	
						// west, north do not work //
						else {
							super.direction = SOUTH;
	
							// south works //
							if (nextFree(current)) return SOUTH;
	
							// west, north, south do not work //
							else {
								super.direction = EAST;
	
								// east works //
								if (nextFree(current)) return EAST;
							}
						}
					}
				}
				if (goal.col > current.col ) {
					super.direction = EAST;
	
					// east works // 
					if (nextFree(current)) return EAST;

					// east does not work //
					else {
						super.direction = NORTH;
	
						// north works //
						if (nextFree(current)) return NORTH;
	
						// east, north do not work //
						else {
							super.direction = SOUTH;
	
							// south works //
							if (nextFree(current)) return SOUTH;
	
							// north, south, east do not work //
							else {
								super.direction = WEST;
	
								// west works //
								if (nextFree(current)) return WEST;
							}
						}
					}
				}
			}
			return direction;
		}
	}
