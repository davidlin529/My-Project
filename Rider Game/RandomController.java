/** @author Summer Ng
 *  @author John David Lin
 *  @version 3/14/2017
 */

import java.util.Random;
import java.lang.Math;

public class RandomController extends CarController {

	private Random rand = new Random();
	private int r;
	private int c;

    public RandomController (CoordInfo oracle) {
        super(oracle);
        direction = roam(ORIGIN);
		r = 0;
		c = 0;
    }

    public void setDefaultDirection(){
        direction = roam(ORIGIN);
    }

    public Coord roam(Coord current){
		int a = rand.nextInt(4);
		if (a == 0) { direction = NORTH; }
		else if (a == 1) { direction = EAST; }
		else if (a == 2) { direction = SOUTH; }
		else { direction = WEST; }
        return direction;
    }

    public Coord drive(Coord current, Coord goal){
		r = goal.row - current.row;
		c = goal.col - current.col;


		if (Math.abs(r) < Math.abs(c)) {
			direction = (new Coord(0, c)).unit();
		}
		else {
			direction = (new Coord(r, 0)).unit();
		}

		Coord newLoc = new Coord((current.row + super.direction.row), (current.col + super.direction.col));
		while (!oracle.coordFree(newLoc)) {
			int a = rand.nextInt(4);
			if (a == 0) { direction = NORTH; }
			else if (a == 1) { direction = EAST; }
			else if (a == 2) { direction = SOUTH; }
			else { direction = WEST; }
			newLoc = new Coord((current.row + super.direction.row), (current.col + super.direction.col));
		}
		return direction;

    }
}
