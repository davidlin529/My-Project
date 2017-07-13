import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

/** @author Summer Ng
 *  @author John David Lin
 *  @version 3/14/2017
 */

public class ManualController extends CarController implements KeyListener{
    public ManualController (CoordInfo oracle) {
        super(oracle);
        direction = ORIGIN;
    }

    public void setDefaultDirection(){
        direction = ORIGIN;
    }

    public Coord roam(Coord current){
        return direction;
    }

    public Coord drive(Coord current, Coord goal){
        return direction;
    }

    @Override
    public void keyPressed (KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_H) {
            if ((direction == NORTH) || (direction == ORIGIN)) {
                direction = WEST;
            }
            else if (direction == WEST) {
                direction = SOUTH;
            }
            else if (direction == SOUTH) {
                direction = EAST;
            }
            else if (direction == EAST) {
                direction = NORTH;
            }

        }
        else if (e.getKeyCode() == KeyEvent.VK_L) {
            if ((direction == NORTH) || (direction == ORIGIN)) {
                direction = EAST;
            }
            else if (direction == EAST) {
                direction = SOUTH;
            }
            else if (direction == SOUTH) {
                direction = WEST;
            }
            else if (direction == WEST) {
                direction = NORTH;
            }
        }
    }

    @Override
    public void keyReleased (KeyEvent e) {}

    @Override
    public synchronized void keyTyped(KeyEvent e) {}
}
