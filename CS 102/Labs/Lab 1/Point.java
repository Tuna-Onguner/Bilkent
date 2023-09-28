/**
 * Point Class ---> Monte Carlo Simulation
 * Creates a point object
 * CS 102, Fall 2021/2022
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 */
public class Point {

    //Properties
    private double x, y;

    //Constructor
    /**
     * Initialize a point in 2D space
     * @param x x-coordinates of the point
     * @param y y-coordinates of the point
     */
    public Point(double x, double y) {
        this.x = x;
        this.y = y;
    }

    //Methods
    /**
     * @return x-coordinates of the point
     */
    public double getX() { return x; }

    /**
     * @param x x-coordinates of the point
     */
    public void setX(double x) {
        this.x = x;
    }

    /**
     * @return y-coordinates of the point
     */
    public double getY() {
        return y;
    }

    /**
     * @param y y-coordinates of the point
     */
    public void setY(double y) {
        this.y = y;
    }

    /**
     * Overriding toString method
     * @return a point object output
     */
    public String toString() {
        return "A(" + getX() + ", " + getY() + ")";
    }
}
