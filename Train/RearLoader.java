/** RearLoader
 * @author John David Lin
 * @version 26 Feb 2017
 */
public class RearLoader implements Loader {
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
        for (int car = (cars.length - 1); car >= 0 ; car --) {
            if (cars[car].canLoad(item))
                return car;
        }
        return Loader.NOCAR; // No car found
    }
}
// vim: ts=4:sw=4:et
