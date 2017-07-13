/**
 * Created by John on 2/25/2017.
 */
public class Test {
    public static void main(String [] args){
        TrainCar trainCar = new TrainCar(9000);
        Item it = new Ford("");
        Item tt = new Ford("focus");
        Item aa = new Electronics("Sys32", 32);
        System.out.println(aa.toString());


        trainCar.load(it);
        trainCar.load(tt);
        trainCar.load(it);
        trainCar.load(aa);
        trainCar.getContents();
        System.out.println("Quantity: " + trainCar.getQuantity(it));
        System.out.println(trainCar.toString());



    }
}
