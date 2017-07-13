/** @author Summer Ng
 *  @author John David Lin
 *  @version 3/13/2017
 */

import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Random;


public class Simulation extends JFrame implements ActionListener, ChangeListener, KeyListener{
    private GraphicsGrid graph;
    private Grid myGrid;
    private JButton newGame;
    private JButton pause;
    private JSlider slider;
    private int tick;
    private TimeTick timeTick;
    private JTextField text;
    private int player;
    private int robot;
    private ManualController m;
    private int numrow;
    private int numcol;
	private int row;
	private int col;
    private SharedCar mycar;
	private int numPickUps = 0;
	private Random rand;
	private int x = 1;
	private int minVal;

    /**
     * use GridSetup to read files
     * @param a file name
     */
	public Simulation (String a) {
	    GridSetup gridSetup = new GridSetup(a);

	    numrow = gridSetup.getDimension().row;
	    numcol = gridSetup.getDimension().col;
        myGrid = new Grid(numrow, numcol);

        Rider rider1 = new Rider();
        rider1.setLocation(gridSetup.getRider());
        if (myGrid.addRider(rider1)) {
            graph.addGridObject(rider1);
        }

        for (int i = 0; i < gridSetup.getObstacles().length; i++) {
            Obstacle obs1 = new Obstacle();
            obs1.setLocation(gridSetup.getObstacles()[i]);
            if (myGrid.addObstacle(obs1)) {
                graph.addGridObject(obs1);
            }
        }

        for (int i = 0; i < gridSetup.getRobocars().length; i++) {
            String cntrl = gridSetup.getControllers()[i];
            CarController conA;
            SharedCar car1;
            switch (cntrl) {
                case "east":
                    conA = new EastWestController(myGrid);
                    car1 = new SharedCar(conA, myGrid);
                    car1.setLocation(gridSetup.getRobocars()[i]);
                    if (myGrid.addCar(car1)) {
                        graph.addGridObject(car1);
                    }
                    break;
                case "north":
                    conA = new NorthSouthController(myGrid);
                    car1 = new SharedCar(conA, myGrid);
                    car1.setLocation(gridSetup.getRobocars()[i]);

                    if (myGrid.addCar(car1)) {
                        graph.addGridObject(car1);
                    }
                    break;
                case "random":
                    conA = new RandomController(myGrid);
                    car1 = new SharedCar(conA, myGrid);
                    car1.setLocation(gridSetup.getRobocars()[i]);
                    if (myGrid.addCar(car1)) {
                        graph.addGridObject(car1);
                    }
                    break;
                default:
                    break;
            }
        }

        m = new ManualController(myGrid);
        mycar = new SharedCar(m, myGrid);
        mycar.setColor(Color.RED);
        mycar.setLocation(gridSetup.getPlayer());
        myGrid.addPlayer(mycar);
        graph.addGridObject(mycar);

        player = 0;
        robot = 0;
        rand = new Random();

        setTitle("Rydr");
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        GridBagConstraints layoutConst;
        setLayout(new GridBagLayout());

        text = new JTextField();
        text.setText("Rider Loaded Player: " + player + " Robots: " + robot);
        text.setEditable(false);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 2;
        layoutConst.gridy = 0;
        layoutConst.gridwidth = 10;
        layoutConst.gridheight = 1;
        add(text, layoutConst);

        newGame = new JButton("New Game");
        newGame.addActionListener(this);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 1;
        layoutConst.gridy = 100;
        add(newGame, layoutConst);

        pause = new JButton("Pause");
        pause.addActionListener(this);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 5, 10, 10);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 3;
        layoutConst.gridy = 100;
        add(pause, layoutConst);

        tick = 100;
        final int min = 10;
        final int max = 100;
        final int unit = 10;
        slider = new JSlider(min, max, unit);
        slider.setValue(10);
        slider.setMajorTickSpacing(10);
        slider.setMinorTickSpacing(1);
        slider.setPaintTicks(true);
        slider.setPaintLabels(true);
        slider.addChangeListener(this);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 5;
        layoutConst.gridy = 100;
        add(slider, layoutConst);

        graph = new GraphicsGrid(numrow, numcol, 50);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 0;
        layoutConst.gridy = 1;
        layoutConst.gridwidth = numcol;
        layoutConst.gridheight = numrow;
        add(graph, layoutConst);

        pack();

        addKeyListener(m);
        addKeyListener(this);

        myGrid.toString();
        timeTick = new TimeTick(tick, myGrid, this);
        timeTick.run();
    }


    /** Constructor
     *  generates the graphics
     * @param numrow number of rows of the grid
     * @param numcol number of columns of the grid
     */
    public Simulation (int numrow, int numcol) {
        this.numrow = numrow;
        this.numcol = numcol;
		this.player = 0;
		this.robot = 0;
		rand = new Random();
        myGrid = new Grid(numrow, numcol);


        setTitle("Rydr");
        setVisible(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        GridBagConstraints layoutConst;
        setLayout(new GridBagLayout());

        text = new JTextField();
        text.setText("Rider Loaded Player: " + player + " Robots: " + robot);
        text.setEditable(false);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 2;
        layoutConst.gridy = 0;
        layoutConst.gridwidth = 10;
        layoutConst.gridheight = 1;
        add(text, layoutConst);

        newGame = new JButton("New Game");
        newGame.addActionListener(this);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 1;
        layoutConst.gridy = 100;
        add(newGame, layoutConst);

        pause = new JButton("Pause");
        pause.addActionListener(this);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 5, 10, 10);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 3;
        layoutConst.gridy = 100;
        add(pause, layoutConst);

        tick = 100;
        final int min = 10;
        final int max = 100;
        final int unit = 10;
        slider = new JSlider(min, max, unit);
        slider.setValue(10);
        slider.setMajorTickSpacing(10);
        slider.setMinorTickSpacing(1);
        slider.setPaintTicks(true);
        slider.setPaintLabels(true);
        slider.addChangeListener(this);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 5;
        layoutConst.gridy = 100;
        add(slider, layoutConst);

        graph = new GraphicsGrid(numrow, numcol, 50);
        layoutConst = new GridBagConstraints();
        layoutConst.insets = new Insets(0, 10, 10, 5);
        layoutConst.fill = GridBagConstraints.HORIZONTAL;
        layoutConst.gridx = 0;
        layoutConst.gridy = 1;
        layoutConst.gridwidth = numcol;
        layoutConst.gridheight = numrow;
        add(graph, layoutConst);

        pack();

        m = new ManualController(myGrid);
        addKeyListener(m);
        addKeyListener(this);

        mycar = new SharedCar(m, myGrid);
        mycar.setColor(Color.RED);
        mycar.setLocation(new Coord(numrow/2, numcol/2));
		myGrid.addPlayer(mycar);
        graph.addGridObject(mycar);

		row = rand.nextInt(numrow);
		col = rand.nextInt(numcol);
        while ( !(myGrid.coordFree(new Coord(row, col))) ) {
            row = rand.nextInt(numrow);
            col = rand.nextInt(numcol);
        }
        Rider rider = new Rider();
        rider.setLocation(new Coord(row, col));
        if (myGrid.addRider(rider)) {
        	graph.addGridObject(rider);
        }
        myGrid.toString();
        timeTick = new TimeTick(tick, myGrid, this);
        timeTick.run();
    }

    /**
     * add objects to the JFrame everytime a rider is picked up
     */
	public void addObj() {
        player = myGrid.playerPicked();
        robot = myGrid.robotPicked();
        text.setText("Rider Loaded Player: " + player + " Robots: " + robot);

        if (numPickUps < (player + robot)) {
            numPickUps = player + robot;

            row = rand.nextInt(numrow);
            col = rand.nextInt(numcol);
            while (!(myGrid.coordFree(new Coord(row, col)))) {
                row = rand.nextInt(numrow);
                col = rand.nextInt(numcol);
            }
            Rider rider2 = new Rider();
            rider2.setLocation(new Coord(row, col));
            myGrid.addRider(rider2);
            graph.addGridObject(rider2);

            row = rand.nextInt(numrow);
            col = rand.nextInt(numcol);
            while (!(myGrid.coordFree(new Coord(row, col)))) {
                row = rand.nextInt(numrow);
                col = rand.nextInt(numcol);
            }
            if ((numPickUps % 10) == 1) {
                int a = rand.nextInt(3);
                CarController con;
                if (a == 0) {
                    con = new RandomController(myGrid);
                }
                else if (a == 1) {
                    con = new NorthSouthController(myGrid);
                }
                else {
                    con = new EastWestController(myGrid);
                }
                SharedCar car = new SharedCar(con, myGrid);
                car.setLocation(new Coord(row, col));
                myGrid.addCar(car);
                graph.addGridObject(car);
                myGrid.toString();
            }
            // new obstacle added every 10th pickup //
            else if (((numPickUps % 10) == 0) && (numPickUps != 0)) {
                if (x < 10){
                    x++;
                }
                slider.setValue(10 * x);
                minVal = slider.getValue();

                System.out.println(tick);

                Obstacle obs = new Obstacle();
                obs.setLocation(new Coord(row, col));
                myGrid.addObstacle(obs);
                graph.addGridObject(obs);
                myGrid.toString();
            }
        }
	}

    /**
     * Make players' car drive when spacebar is pressed
     * @param e KeyEvent
     */
	@Override
    public void keyPressed (KeyEvent e) {
        if (e.getKeyCode() == KeyEvent.VK_SPACE) {
            mycar.drive();
        }
    }

    /**
     *
     * @param e KeyEvent
     */
    @Override
    public void keyReleased (KeyEvent e) {}

    /**
     *
     * @param e KeyEvent
     */
    @Override
    public void keyTyped (KeyEvent e) {}

    /**
     * Update the graphics, JPanel
     */
    protected void update () {
        graph.repaint();
        graph.removeGridObject(myGrid.removeRider());
        requestFocus();
        addObj();
    }

    /**
     *  perform actions upon pressing the buttons
     *  pause - pauses the game
     *  new game - creates a new game
     * @param event ActionEvent
     */
    @Override
    public void actionPerformed(ActionEvent event) {
        JButton sourceEvent = (JButton) event.getSource();
        if (sourceEvent == newGame) {
            graph.clearObjects();
            myGrid.clearAll();
            numPickUps = 0;

            timeTick.stop();
            timeTick = new TimeTick(tick, myGrid, this);
            Thread t = new Thread(timeTick);
            t.start();

            mycar = new SharedCar(m, myGrid);
            mycar.setColor(Color.RED);
            mycar.setLocation(new Coord(numrow/2, numcol/2));
			myGrid.addPlayer(mycar);
            graph.addGridObject(mycar);

			row = rand.nextInt(numrow);
			col = rand.nextInt(numcol);
			while ( !(myGrid.coordFree(new Coord(row, col))) ) {
                row = rand.nextInt(numrow);
                col = rand.nextInt(numcol);
            }
			Rider rider = new Rider();
        	rider.setLocation(new Coord(row, col));
        	myGrid.addRider(rider);
        	graph.addGridObject(rider);
			update();
        	myGrid.toString();

            player = myGrid.playerPicked();
            robot = myGrid.robotPicked();
            text.setText("Rider Loaded Player: " + player + " Robots: " + robot);
        }

        else if (sourceEvent == pause) {
            final int INFINITY = 999999999;
            if (!timeTick.paused()) {
                timeTick.changeState();
                timeTick.setTicks(INFINITY);
                pause.setText("Resume");
            }
            else {
                timeTick.changeState();
                timeTick.setTicks(tick);
                pause.setText("Pause");
            }
        }
    }

    /**
     *  reads input from the slider
     *  @param event ChangeEvent
     */
    @Override
    public void stateChanged(ChangeEvent event) {
        if (slider.getValue() > minVal) {
            tick = 110 - slider.getValue();
            timeTick.setTicks(tick);
        }
        else {
            slider.setValue(minVal);
            tick = 110 - x * 10;
        }
    }

    public static void main(String[] args) {
        Simulation myMain = new Simulation(10, 10);
    }
}
