/**
 * Circle Class ---> Monte Carlo Simulation
 * Creates a circle object
 * CS 102, Fall 2021/2022
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 */
public class Circle {

    //Properties
    private double radius;
    private Point center;

    //Constructor
    /**
     * Initialize a circle
     * @param radius radius of the circle
     * @param center center point of the circle
     */
    public Circle(double radius, Point center) {
        this.radius = radius;
        this.center = center;
    }

    //Methods
    /**
     * @return the radius of the circle
     */
    public double getRadius() {
        return radius;
    }

    /**
     * @param radius radius of the circle
     */
    public void setRadius(double radius) {
        this.radius = radius;
    }

    /**
     * @return center point of the circle
     */
    public Point getCenter() {
        return center;
    }

    /**
     * @param center center of the circle
     */
    public void setCenter(Point center) {
        this.center = center;
    }

    /**
     * Checking whether the point is inside the circle
     * @param thePoint a random point
     * @return true if it is in the circle
     */
    public boolean contains( Point thePoint ) {
        return  Math.pow((thePoint.getX() - center.getX()), 2) +
                Math.pow((thePoint.getY() - center.getY()), 2) < Math.pow(getRadius(), 2);
    }

    /**
     * Overriding toString method
     * @return a circle object output
     */
    public String toString() {
        return  "Center of the circle: " + center +
                "Radius of the circle: " + radius;
    }
}
