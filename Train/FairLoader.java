/** FairLoader
 * @author John David Lin
 * @version 26 Feb 2017
 */
public class FairLoader implements Loader{
    /** choose which car can accommodate the item
     *  @param cars Array of TrainCars
     *  @param item What to load
     *  @return car index in range [0..cars.size) if item fits
     *         Loader.NOCAR if item cannot be loaded.
     *         Loader.NOCAR if cars is null
     *         Loader.NOCAR if item is null
     */
    public int chooseCar(TrainCar [] cars, Item item) {
        if (cars == null) return Loader.NOCAR;
        if (item == null) return Loader.NOCAR;
        int minCar = -1;
        double minWeight = 0.00;
        for (int car = 0; car < cars.length; car ++) {
            if (cars[car].canLoad(item)){
                if ((minCar == -1) || (minWeight > cars[car].getWeight())) {
                    minCar = car;
                    minWeight = cars[car].getWeight();
                }
            }
        }
        if (minCar == -1) {
            return Loader.NOCAR; // No car found
        }
        else
            return minCar;
    }
}
