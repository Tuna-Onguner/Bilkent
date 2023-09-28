package shapes;

/**
 * =============================================================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Shape Class:
 * The abstract super class of the three shapes (Rectangle, Square, and Circle)
 * Implements "Locatable" Inheritance
 * Initializing the coordinates and the area of shapes
 * =============================================================================
 */
public abstract class Shape implements Locatable, Drawable {

    //Properties
    public int x, y; //Coordinates (x, y)

    //Methods
    /**
     * ================================
     * Calculating the area of a shape
     * @return area of the shape
     * ================================
     */
    abstract double getArea();
}
