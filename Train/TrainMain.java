/**
 * @author John David Lin
 * @version 26 Feb 2017
 */
import java.util.Scanner;
public class TrainMain {

    // These use Scanner methods to check if a string will be valid
    // as a particular number (to avoid errors with parseDouble/parseInt)
    private boolean isInt(String s) {
        Scanner scnr = new Scanner(s);
        return scnr.hasNextInt();
    }
    private boolean isDouble(String s) {
        Scanner scnr = new Scanner(s);
        return scnr.hasNextDouble();
    }

    private boolean checkBadManifestUsage(String[] parts) {
        return !((parts.length == 1) ||
                (parts.length == 2 && isInt(parts[1])));
    }
    private boolean checkBadCarUsage(String[] parts) {
        return !((parts.length == 3) && isInt(parts[1]) && isDouble(parts[2]));
    }
    private boolean checkBadFordUsage(String[] parts) {
        return !((parts.length == 3) && isInt(parts[2]));
    }
    private boolean checkBadElectronicsUsage(String[] parts) {
        return !((parts.length == 4) && isDouble(parts[2]) && isInt(parts[3]));
    }

    private void printBadManifestUsage() {
        System.out.println("Usage 1: manifest\n  Prints a summary of the whole train.");
        System.out.println("Usage 2: manifest car\n  Prints a summary of the given car (an int)");
    }
    private void printBadCarUsage() {
        System.out.println("Usage: add-cars n w\n  Creates a n new cars with max weight capacity w");
    }
    private void printBadFordUsage() {
        System.out.println("Usage: ford s n\n  Creates and loads n new Fords with the given model name s");
    }
    private void printBadElectronicsUsage() {
        System.out.println("Usage: electronics s w n\n  Creates and loads n new Electronics objects with the given description s and weight w");
    }
    private void printUnknownCommand(String[] parts) {
        System.out.println("Unknown command: " + parts[0]);
    }

    private void loadItem(Train train, Item i, int howMany) {
        int numLoaded = train.load(howMany, i);
        if(numLoaded == 0) {
            System.out.println("Unable to load item");
        }
        else if(numLoaded == howMany) {
            System.out.println(numLoaded + " items successfully loaded");
        }
        else {
            System.out.println(numLoaded + " of " + howMany + " items successfully loaded");
        }
    }

    private void runInterpreter() {
        Scanner scan = new Scanner(System.in);
        Loader loader = null;
        System.out.print("What loader would you like to use (front/rear/fair/robin, default front)");

        String loaderName = scan.nextLine();
        switch(loaderName) {
            case "":
            case "front":
                loader = new FrontLoader();
                break;
            case "rear":
                loader = new RearLoader();
                break;
            case "robin":
                loader = new RoundRobinLoader();
                break;
            case "fair":
                loader = new FairLoader();
                break;
            default:
                System.err.println("Invalid loader " + loaderName + ", exiting.");
                return;
        }

        Train train = new Train(loader);

        String cmd = "";

        int howMany;

        while(true) {
            System.err.print("> ");
            if(!scan.hasNext()) { return; }
            cmd = scan.nextLine();
            String[] parts = cmd.split("\\s+");
            if(parts.length == 0) { continue; }
            switch(parts[0]) {
                case "manifest":
                    if(checkBadManifestUsage(parts)) {
                        printBadManifestUsage();
                        continue;
                    }
                    if(parts.length == 1) {
                        Item[][] contents = train.getContents();
                        System.out.println("Total weight: " + train.getWeight());
                        System.out.println("Cars: " + train.getNumCars());
                        if(contents == null) {
                            System.out.println("train.getContents() was null; cannot print manifest");
                            continue;
                        }

                        for(int car = 0; car < contents.length; car += 1) {
                            System.out.println("Car #" + car + ":");
                            System.out.println("  " + train.getWeight(car) + " of " + train.getTrainCar(car).getMaxWeight() + "kg");
                            System.out.println("  " + train.getContents(car).length + " items in car");
                        }
                    }
                    else if(parts.length == 2) {
                        int car = Integer.parseInt(parts[1]);
                        if(car >= train.getNumCars() || car < 0) {
                            System.out.println("No such car (there are " + train.getNumCars() + " cars).");
                            continue;
                        }
                        Item[] items = train.getContents(car);
                        if(items == null) {
                            System.out.println("train.getContents(car) was null; cannot print manifest for the given car");
                            continue;
                        }
                        System.out.println("Total car weight: " + train.getWeight(car));
                        System.out.println("Items: " + items.length);
                        for(int item = 0; item < items.length; item += 1) {
                            System.out.println("  " + items[item].description());
                        }
                    }
                    continue;

                case "add-cars":
                    if(checkBadCarUsage(parts)) {
                        printBadCarUsage();
                        continue;
                    }
                    train.addCars(Integer.parseInt(parts[1]), Double.parseDouble(parts[2]));
                    continue;

                case "ford":
                    if(checkBadFordUsage(parts)) {
                        printBadFordUsage();
                        continue;
                    }
                    String model = parts[1];
                    howMany = Integer.parseInt(parts[2]);
                    Item theFord = new Ford(model);
                    loadItem(train, theFord, howMany);
                    continue;

                case "electronics":
                    if(checkBadElectronicsUsage(parts)) {
                        printBadElectronicsUsage();
                        continue;
                    }
                    String desc = parts[1];
                    double weight = Double.parseDouble(parts[2]);
                    howMany = Integer.parseInt(parts[3]);
                    Item theElectronics = new Electronics(desc, weight);
                    loadItem(train, theElectronics, howMany);
                    continue;

                case "exit":
                    break;

                default:
                    printUnknownCommand(parts);
                    continue;
            }
            break;
        }
    }
    public static void main(String[] args) {
        TrainMain myMain = new TrainMain();
        myMain.runInterpreter();
    }
}
