/**
 * Rectangle Class ---> Monte Carlo Simulation
 * Creates a rectangle object
 * CS 102, Fall 2021/2022
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 */
public class Rectangle {

    //Properties
    private double x, y, width, height;
    private Point theCorner;

    //Constructor
    /**
     * Initialize a rectangle
     * @param width width of the rectangle
     * @param height height of the rectangle
     * @param theCorner corner point of the rectangle
     */
    public Rectangle(double width, double height, Point theCorner) {
        this.width = width;
        this.height = height;
        this.theCorner = theCorner;
    }

    //Methods
    /**
     * @return x-coordinates of the corner of the rectangle
     */
    public double getX() {
        return x;
    }

    /**
     * @param x x-coordinates of the corner of the rectangle
     */
    public void setX(double x) {
        this.x = x;
    }

    /**
     * @return y-coordinates of the corner of the rectangle
     */
    public double getY() {
        return y;
    }

    /**
     * @param y y-coordinates of the corner of the rectangle
     */
    public void setY(double y) {
        this.y = y;
    }

    /**
     * @return the width of the rectangle
     */
    public double getWidth() {
        return width;
    }

    /**
     * @param width width of the rectangle
     */
    public void setWidth(double width) {
        this.width = width;
    }

    /**
     * @return the height of the rectangle
     */
    public double getHeight() {
        return height;
    }

    /**
     * @param height height of the rectangle
     */
    public void setHeight(double height) {
        this.height = height;
    }

    /**
     * @return the corner of the rectangle
     */
    public Point getTheCorner() {
        return theCorner;
    }

    /**
     * @param theCorner corner point of the rectangle
     */
    public void setTheCorner(Point theCorner) {
        this.theCorner = theCorner;
    }

    /**
     * Checking whether the point is inside the rectangle
     * @param thePoint a random point
     * @return true if the point inside the rectangle
     */
    public boolean contains( Point thePoint ) {
        return  thePoint.getX() >= theCorner.getX() &&
                thePoint.getX() <= theCorner.getX() + getWidth() &&
                thePoint.getY() >= theCorner.getY() &&
                thePoint.getY() <= theCorner.getY() + getHeight();
    }

    /**
     * Creating random points inside the rectangle
     * @return new point inside the rectangle
     */
    public Point getRandomPoints() {
        double pointX = Math.random() * getWidth();
        double pointY = Math.random() * getHeight();
        if (contains(new Point(pointX, pointY)))
            return new Point(pointX, pointY);
        return null;
    }

    /**
     * Overriding toString method
     * @return a rectangle object output
     */
    public String toString() {
        return  "Corner of the rectangle: " + theCorner +
                "\nWidth of the rectangle: " + width +
                "\nHeight of the rectangle: " + height;
    }
}
