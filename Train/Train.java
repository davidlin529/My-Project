/**
 * @author John David Lin
 * @version 26 Feb 2017
 */
import java.util.ArrayList;
public class Train {
    private ArrayList<TrainCar> train = new ArrayList<>();
    private TrainCar[] trainArray = new TrainCar[0];
    private Loader loader;

    /** Constructor
     * @param loadMaster - any object that implements the Loader interface.
     *                     a null reference results in runtime errors and
     *                     does not need to be checked.
     */
    public Train(Loader loadMaster) {
        loader = loadMaster;
    }

    /** Getter - Return the number of TrainCars in this Train
     * @return  how many TrainCars have been added to this Train
     */
    public int getNumCars() {
        return train.size();
    }

    /** Getter -  Return the total weight of all cargo in all TrainCars
     * @return  total weight of cargo, summed over all TrainCars
     */
    public double getWeight() {
        int sumWeight = 0;
        for (int i = 0; i < train.size(); ++i) {
            sumWeight += train.get(i).getWeight();
        }
        return sumWeight;
    }

    /** Getter - Return the weight of all cargo in a particular TrainCar
     * @param car  car number starting from 0
     * @return  weight of TrainCar indicated by car. Return 0 if car
     *           number does not exist
     */
    public double getWeight(int car) {
        if ((car >= train.size()) || (car < 0)){
            return 0;
        }
        else {
            return train.get(car).getWeight();
        }
    }

    /** Getter - 2D array of Items that lists all cargo
     * @return 2D array of Items contained in each TrainCar. Row index
     *         is the car#.
     */
    public Item [][] getContents() {
        int column = 0;
        for (int i = 0; i < train.size(); ++i){
            if (column < train.get(i).getContents().length){
                column = train.get(i).getContents().length;
            }
        }
        Item[][] trainContent = new Item[train.size()][column];

        for (int i = 0; i < train.size(); ++i) {
            for (int j = 0; j < train.get(i).getContents().length; ++j){
                trainContent[i][j] = train.get(i).getContents()[j];
            }
        }
        return trainContent;
    }

    /** Getter - 1D array of Items that lists all cargo of a particular
     *           TrainCar
     * @param car car number starting from 0
     * @return 1D array of Items contained in the specif TrainCar.
     *         If car number does not exist, return null.
     */
    public Item [] getContents(int car) {
        if ((car >= train.size()) || (car < 0)){
            return null;
        }
        else {
            return train.get(car).getContents();
        }
    }

    /** Getter - Return TrainCar reference for specific car #
     * @param car car number starting from 0
     * @return reference to the TrainCar instance at car index
     *         If car number does not exist, return null.
     */
    public TrainCar getTrainCar(int car) {
        if ((car >= train.size()) || (car < 0)){
            return null;
        }
        else {
            return train.get(car);
        }
    }

    /** Add TrainCars to the Train, all with identical capacity
     * @param nCars  how many to add. Must be positive
     * @param maxWeight cargo capacity of the TrainCars to be added.
     *                  Must be nonnegative
     *
     * <p> cars are numbered sequentially from 0.  The Nth car added has
     * index (N-1). (array ordering). addCars may be invoked any number
     * of times.
     */
    public void addCars(int nCars, double maxWeight) {
        if ((maxWeight >= 0) || (nCars > 0)){
            for (int i = 0; i < nCars; ++i) {
                TrainCar trainCar = new TrainCar(maxWeight);
                train.add(trainCar);
            }
        }
    }

    /** load item onto train
     *  @param item any object that implements Item
     *  @return 0/1. 0 if it could not load, 1 if it loads.
     *          returns 0 if item is null.
     *
     *  <p>
     *  This method uses the Loader to determine which car in which
     *  to load an item. The loader might return Loader.NOCAR
     *  If a valid car index is returned by the Loader, then this
     *  method loads the item into the specific car.
     *
     */
    public int load(Item item) {
        int car = loader.chooseCar(train.toArray(trainArray), item);
        if ((item == null) || (car == Loader.NOCAR)) {
            return 0;
        }
        else {
            train.get(car).load(item);
            return 1;
        }
    }

    /** Convenience wrapper method around load
     *  @param qty number of items to load
     *  @param item any object that implements Item
     *  @return number of items successfully loaded [0..qty]
     *          returns 0 if item is null
     */
    public int load(int qty, Item item) {
        int counter = 0;
        if (item == null){
            return 0;
        }
        else{
            for (int i = 0; i < qty; ++i) {
                int car = loader.chooseCar(train.toArray(trainArray), item);
                if (car != Loader.NOCAR){
                    train.get(car).load(item);
                    counter ++;
                }
            }
            return counter;
        }
    }
}
// vim: ts=4:sw=4:et
