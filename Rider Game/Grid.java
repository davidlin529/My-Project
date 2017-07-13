import java.util.ArrayList;

/** @author Summer Ng
 *  @author John David Lin
 *  @version 3/13/2017
 */
public class Grid implements GridInfo, CoordInfo{
    private ArrayList<GridObject> All = new ArrayList<>();
    private ArrayList<Rider> Rider = new ArrayList<>();
	private ArrayList<Rider> LoadedRiders = new ArrayList<>();
    private ArrayList<SharedCar> Car = new ArrayList<>();
    private ArrayList<SharedCar> Player = new ArrayList<>();
    private Rider riderToRemove;

    private int row;
    private int col;
	private int pickedByPlayer = 0;
	private int pickedByRobot = 0;

    /** Constructor
     *
     * @param row number of rows of the grid
     * @param col number of columns of the grid
     */
    public Grid (int row, int col) {
        this.row = row;
        this.col = col;
    }

    /** Determine if the coordinate is free
     * @param loc location to query
     * @return true if coordinate is free, false otherwise, false if coordinate is null
     */
    public boolean coordFree (Coord loc) {
        if (loc == null) {
            return false;
        }
        else if ((loc.row < row) && (loc.row >= 0) && (loc.col < col) && (loc.col >= 0)) {
            for (int i = 0; i < All.size(); ++i) {
                if (loc.equals(All.get(i).getLocation())) {
                    return false;
                }
            }
            return true;
        }
        else  {
            return false;
        }
    }

    /** For cars to claim a location
     *
     * @param car Sharedcar
     * @param loc location to be claimed
     * @return true is successfully claimed, false if otherwise
     */
    public boolean claim(SharedCar car, Coord loc) {
        if (coordFree(loc)){
            car.setLocation(loc);
            if (!All.contains(car)) {
                All.add(car);
            }
            return true;
        }
        else {
            return false;
        }
    }

    /** determine if the rider is loaded into the car
     *
     * @param car SharedCar
     * @return true if the car picks up the rider, false otherwise
     */
    public boolean riderLoaded(SharedCar car) {
        loadRider();
        for (int i = 0; i < Rider.size(); i++) {
            if (car.getLocation().equals(Rider.get(i).getLocation())) {
                Rider.get(i).pickUp(car);
				LoadedRiders.add(Rider.get(i));
                riderToRemove = Rider.get(i);
                Rider.remove(i);
                if (Rider.size() == 0) {
                    allRoam();
                    return true;
                }
            }
        }
        return false;
    }

    /** add riders to the internal ArrayList
     *
     * @param rider rider to be added
     * @return true if added successfully, false otherwise
     */
    protected boolean addRider(Rider rider) {
        if (coordFree(rider.getLocation())) {
            for (int i = 0; i < Rider.size(); i++) {
                if (rider.getLocation().equals(Rider.get(i).getLocation())) {
                    return false;
                }
            }
            Rider.add(rider);
            loadRider();
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * @return the rider that is loaded into the car to be removed from the graphics
     */
    protected Rider removeRider () {
        return riderToRemove;
    }

    /** make all cars attempt to get the rider
     *
     */
    private void loadRider () {
        if (Rider.size() != 0) {
            Player.get(0).newRider(Rider.get(0).getLocation());
            for (int i = 0; i < Car.size(); i++) {
                Car.get(i).newRider(Rider.get(0).getLocation());
            }
        }
    }

    /** Add cars to the internal ArrayList
     *
     * @param car car to be added
     * @return true if added successfully, false otherwise
     */
    protected boolean addCar(SharedCar car) {
        if (coordFree(car.getLocation())) {
            for (int i = 0; i < Rider.size(); i++) {
                if (car.getLocation().equals(Rider.get(i).getLocation())) {
                    return false;
                }
            }
            All.add(car);
            Car.add(car);
            if (Rider.size() != 0) {
                loadRider();
            }
            return true;
        }
        else {
            return false;
        }
    }

    /** Add players to the internal ArrayList
     *
     * @param car the player's car to be added
     */
    protected void addPlayer (SharedCar car) {
        All.add(car);
        Player.add(car);
    }

    /** Add obstacles to the internal ArrayList
     *
     * @param obs obstacles to be added
     * @return true if added successfully, false otherwise
     */
    protected boolean addObstacle(Obstacle obs) {
        if (coordFree(obs.getLocation())) {
            for (int i = 0; i < Rider.size(); i++) {
                if (obs.getLocation().equals(Rider.get(i).getLocation())) {
                    return false;
                }
            }
            All.add(obs);
            return true;
        }
        else {
            return false;
        }
    }

    /**
     *  make all cars drive
     */
    protected void allDrive() {
        for (int i = 0; i < Car.size(); ++i) {
            Car.get(i).drive();
        }
		Player.get(0).drive();
    }

    /**
     * make all cars roam
     */
    protected void allRoam() {
        for (int i = 0; i < Car.size(); ++i) {
            Car.get(i).roam();
        }
		Player.get(0).roam();
    }

    /**
     * @return the number of riders picked up by the player
     */
	protected int playerPicked() {
        pickedByPlayer = 0;
        for (int i = 0; i < LoadedRiders.size(); i++) {
            if (LoadedRiders.get(i).getDriver() == Player.get(0)) {
                pickedByPlayer++;
            }
        }
        return pickedByPlayer;
	}

    /**
     *
     * @return the number of riders picked up by the robots
     */
	protected int robotPicked() {
        pickedByRobot = 0;
		pickedByRobot = LoadedRiders.size() - pickedByPlayer;
		return pickedByRobot;
	}

    /**
     *
     * @return pretty print of the grid
     */
    public String toString() {
		System.out.println("Rider array size " + Rider.size());
		System.out.println("Player: " + pickedByPlayer + " Robot: " + pickedByRobot);
		System.out.println("Loaded Rider array size " + LoadedRiders.size());

        for (int l = 0; l < row + 2; l++) {
            System.out.print("=");
        }
        System.out.println("");
        for (int i = 0; i < row; i++) {
            System.out.print("|");
            for (int j = 0; j < col; j++) {
                Coord loc = new Coord(i, j);
                if (coordFree(loc)) {
                    boolean printSpace = true;
                    for (int l = 0; l < Rider.size(); l++) {
                        if (loc.equals(Rider.get(l).getLocation())) {
                            System.out.print(Rider.get(l).getSymbol());
                            printSpace = false;
                        }
                    }
                    if (printSpace) {
                        System.out.print(" ");
                    }
                }
                else {
                    for (int k = 0; k < All.size(); k++) {
                        if (loc.equals(All.get(k).getLocation())) {
                            System.out.print(All.get(k).getSymbol());
                        }
                    }
                }
            }
            System.out.println("|");
        }
        for (int l = 0; l < row + 2; l++) {
            System.out.print("=");
        }
        System.out.println("");
        return "";
    }

    /**
     * updates the grid
     */
    protected void update() {
        allDrive();
        toString();
    }

    /**
     * clears the grid for a new game
     */
    protected void clearAll () {
        Rider.clear();
        All.clear();
        Car.clear();
		Player.clear();
		LoadedRiders.clear();
		pickedByPlayer = 0;
		pickedByRobot = 0;
    }
}
