/**
 * @author John David Lin
 * @version 26 Feb 2017
 */
import java.util.ArrayList;
public class TrainCar extends java.lang.Object{
    private double maxWeight = 0;
    private double currentWeight = 0;
    private ArrayList<Item> cargo = new ArrayList<>();
    private Item[] myArray = new Item[0];

    /** Constructor
     * @param maxW - maximum cargo weight (KG) of this TrainCar. if maxW &lt;=0, then
     *               max cargo weight is 0 KG
     */
    public TrainCar(double maxW){
        if (maxW <= 0){
            maxWeight = 0;
        }
        else {
            maxWeight = maxW;
        }
    }

    /** determine if item can be loaded in car
     *
     * @param item - object to be tested for loading
     * @return true if item would not put this TrainCar over its maximum weight.
     * false otherwise. Return false if item is null. Return false if item weight &lt;= 0.
     */
    public boolean canLoad(Item item){
        if (item == null){
            return false;
        }
        else {
            if (((maxWeight - currentWeight) < item.getWeight()) || (item.getWeight() > maxWeight) || (item.getWeight() <= 0)){
                return false;
            }
            else {
                return true;
            }
        }
    }

    /** Return an array of Item references of what is loaded in this car
     *
     * @return Array of Items currently loaded in this car. length of array indicates
     * the number of items loaded. This may be a reference or a deep copy.
     */
    public Item[] getContents (){
        return cargo.toArray(myArray);
    }

    /** Getter - return maximum weight of cargo in this TrainCar
     *
     * @return Maximum weight of cargo in this TrainCar (in KG)
     */
    public double getMaxWeight (){
        return maxWeight;
    }

    /** Determine how many of a particular Item is in this TrainCar
     *
     * @param item - Reference to any object that implements Item
     * @return number of items in the TrainCar. null item should return 0
     */
    public int getQuantity (Item item) {
        int counter = 0;
        if (item == null) {
            return 0;
        }
        else {
            for (int i = 0; i < cargo.size(); ++i) {
                if (cargo.get(i) == item) {
                    counter += 1;
                }
            }
            return counter;
        }
    }

    /** Getter - return current weight of cargo in this TrainCar
     *
     * @return current weight of cargo in this TrainCar (in KG)
     */
    public double getWeight (){
        return currentWeight;
    }

    /** load an item into the TrainCar
     *
     * @param item - any object that implements Item
     * @return 0/1. return 0 if item not loaded, 1 if loaded. returns 0 if item is
     * null. returns 0 if canLoad would return false.
     */
    public int load (Item item){
        if ((item == null) || (!canLoad(item))){
            return 0;
        }
        else {
            cargo.add(item);
            currentWeight += item.getWeight();
            return 1;
        }
    }

    @Override
    public String toString (){
        return "Max Weight: " + getMaxWeight() + "\nWeight: " + getWeight();
    }
}
