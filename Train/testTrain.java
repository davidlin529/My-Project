/**
 * Created by John on 2/26/2017.
 */
public class testTrain {
    public static void main(String [] args) {
        FrontLoader loader = new FrontLoader();
        Train train = new Train(loader);
        Item tt = new Ford("focus");

        train.addCars(1, 1500);
        System.out.println("loaded: " + train.load(tt));
        System.out.println("loaded: " + train.load(3, tt));
        System.out.println("NumCars: " + train.getNumCars());
        System.out.println("Total weight: " + train.getWeight());
        System.out.println("Weight of a train car: " + train.getWeight(0));
        System.out.println("train content: " + train.getContents());
        System.out.println("content of a car: " + train.getContents(1));

    }




}
