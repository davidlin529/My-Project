public class SimpleMain {
    public static void main(String [] args){
        Grid myGrid = new Grid(10, 10);
        CarController conA = new ManualController(myGrid);
        CarController conB = new RandomController(myGrid);

        Rider rider1 = new Rider();
        Rider rider2 = new Rider();
        SharedCar car1 = new SharedCar(conA, myGrid);
        SharedCar car2 = new SharedCar(conB, myGrid);
        Obstacle obs1 = new Obstacle();
        Obstacle obs2 = new Obstacle();
        Obstacle obs3 = new Obstacle();

        rider1.setLocation(new Coord(0, 0));
        rider2.setLocation(new Coord(6, 0));
        car1.setLocation(new Coord(0, 0));
        car2.setLocation(new Coord(7, 2));
        obs1.setLocation(new Coord(5, 3));
        obs2.setLocation(new Coord(6, 3));
        obs3.setLocation(new Coord(7, 6));

        myGrid.addPlayer(car1);
        myGrid.addRider(rider2);
        myGrid.addCar(car2);
        myGrid.addRider(rider1);
        myGrid.addObstacle(obs1);
        myGrid.addObstacle(obs2);
        myGrid.addObstacle(obs3);

        for (int i = 0; i < 20; i++) {
            myGrid.toString();
            myGrid.allDrive();
        }

    }
}
