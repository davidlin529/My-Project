/** Interface to be a Loader object.
 */
public interface Loader {
    public static final int NOCAR = -1;
    /** Choose an appropriate car according to a Loader's specific
     *  algorithm
     *  @param cars Array of TrainCars
     *  @param item What to load
     *  @return car index in range [0..cars.size) if item fits.
     *         Loader.NOCAR if item cannot be loaded.
     *         Loader.NOCAR if cars is null.
     *         Loader.NOCAR if item is null.
     */
    public int chooseCar(TrainCar [] cars, Item item);
}
// vim: ts=4:sw=4:et
