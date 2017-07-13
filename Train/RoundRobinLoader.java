/** RoundRobinLoader
 * @author John David Lin
 * @version 26 Feb 2017
 */
public class RoundRobinLoader implements Loader{
    int chooseCar = 0;
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


        int fullCar = Loader.NOCAR;
        while (chooseCar != fullCar) {
            if (cars[chooseCar].canLoad(item)) {
                fullCar = Loader.NOCAR;
                int car = chooseCar;
                chooseCar ++;
                if (chooseCar == cars.length) {
                    chooseCar = 0;
                }
                return car;
            }
            else {
                if (fullCar == Loader.NOCAR) {
                    fullCar = chooseCar;
                }
            }
            chooseCar ++;
            if (chooseCar == cars.length) {
                chooseCar = 0;
            }
        }
        return Loader.NOCAR; // No car found
    }
}
