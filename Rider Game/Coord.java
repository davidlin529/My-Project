/** @author Summer Ng
 *  @author John David Lin
 */

import java.lang.Math;

public class Coord extends java.lang.Object implements java.lang.Comparable<Coord> {
    public final int row;
    public final int col;

    public Coord() {
        row = 0;
        col = 0;
    }

    public Coord(int row1, int col1) {
        row = row1;
        col = col1;
    }

    public Coord(Coord other) {
        row = other.row;
        col = other.col;
    }

    public Coord add(Coord b) {
        if (b == null) {
            return null;
        }
        else {
            return new Coord (row + b.row, col + b.col);
        }
    }

    public int compareTo(Coord other) {
        if (other == null) {
            return -1;
        }
        if (other.equals(new Coord(0, 0))){
            return 1;
        }

        int a = (this.row * this.row) + (this.col * this.col);
        int b = (other.row * other.row) + (other.col * other.col);
        System.out.println(a + " : " + b);

        if (a == b) {
            return 0;
        }
        else if (a > b) {
            return 1;
        }
        else if (a < b) {
            return -1;
        }
        return 0;
    }

    public Coord diff(Coord b) {
        if (b == null) {
            return null;
        }
        else {
            return new Coord (row - b.row, col - b.col);
        }
    }

    public Coord dist(Coord b) {
        if (b == null) {
            return null;
        }
        else {
            return new Coord (Math.abs(row - b.row), Math.abs(col - b.col));
        }
    }

    public int dist2(Coord b) {
        if (b == null) {
            return Integer.MAX_VALUE;
        }
        else {
            return (dist(b).row * dist(b).row) + (dist(b).col * dist(b).col);
        }
    }

    @Override
    public boolean equals(java.lang.Object other) {
        if (other instanceof Coord) {
            if ((this.row == ((Coord)other).row) && (this.col == ((Coord)other).col)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public java.lang.String toString() {
        return "".format ("Coord:(row=%d,col=%d)", row, col);
    }

    public Coord unit() {
        if (this.row == 0) {
            if (this.col == 0) {
                return new Coord (0,0);
            }
            else if (this.col < 0) {
                return new Coord (0, -1);
            }
            else {
                return new Coord (0, 1);
            }

        }
        else if (this.row < 0) {
            if (this.col == 0) {
                return new Coord (-1,0);
            }
            else if (this.col < 0) {
                return new Coord (-1, -1);
            }
            else {
                return new Coord (-1, 1);
            }
        }
        else {
            if (this.col == 0) {
                return new Coord (1,0);
            }
            else if (this.col < 0) {
                return new Coord (1, -1);
            }
            else {
                return new Coord (1, 1);
            }
        }
    }
}
