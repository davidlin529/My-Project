/** FrontLoader
 *  Load into the first car in which an item will fit.
 *  Always start from the front of the train
 */
public class FrontLoader implements Loader {
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
        for (int car = 0; car < cars.length; car ++) {
            if (cars[car].canLoad(item))
                return car;
        }
        return Loader.NOCAR; // No car found
    }
}
// vim: ts=4:sw=4:et
