package shapes;
import java.awt.*;

/**
 * =======================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Rectangle Class:
 * Initialing a circle shape object
 * =======================================
 */
public class Circle extends Shape implements Selectable {

    //Properties
    private int radius;
    private boolean isSelected;

    //Constructor
    /**
     * =============================================
     * Constructor --- Circle Class
     * Creating a circle shape object
     * @param radius the radius of the circle shape
     * =============================================
     */
    public Circle(int radius) {
        super();
        this.radius = radius;
    }

    //Methods
    /**
     * =================================
     * Calculating the area of a circle
     * @return area of the shape
     * =================================
     */
    @Override
    double getArea() {
        return (Math.PI * Math.pow(radius, 2));
    }

    public int getRadius() {
        return radius;
    }

    public void setRadius(int radius) {
        this.radius = radius;
    }

    /**
     * ====================================================
     * Overriding toString Method
     * @return a proper string representation of a circle
     * ====================================================
     */
    @Override
    public String toString() {
        String isSelectedStr = "Not Selected";
        if (isSelected)
            isSelectedStr = "Selected";
        return "Circle (radius: " + radius + ")" +
                "[x-coordinate: " + x + ", y-coordinate: " + y + "] " + isSelectedStr + "\n";
    }

    //Locatable Methods
    /**
     * ====================================
     * Getting the x-coordinate of a shape
     * @return x-coordinate of the shape
     * ====================================
     */
    @Override
    public int getX() {
        return x;
    }

    /**
     * ====================================
     * Getting the y-coordinate of a shape
     * @return y-coordinate of the shape
     * ====================================
     */
    @Override
    public int getY() {
        return y;
    }

    /**
     * ======================================================================
     * Locating a shape into a position according to the stated coordinates
     * @param x stated x-coordinate
     * @param y stated y-coordinate
     * ======================================================================
     */
    @Override
    public void setLocation(int x, int y) {
        this.x = x;
        this.y = y;
    }

    //Selectable Methods
    /**
     * =========================================================================
     * Getting the selected situation of a shape ("selected" or "not selected")
     * @return selected situation
     * =========================================================================
     */
    @Override
    public boolean getSelected() {
        return isSelected;
    }

    /**
     * ============================================================================
     * Setting the selected situation of a shape according to the stated parameter
     * @param bln the new situation of the shape object
     * ============================================================================
     */
    @Override
    public void setSelected(boolean bln) {
        isSelected = bln;
    }

    /**
     * =====================================================================
     * Creating and returning to a new shape that contains the stated point
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     * @return a new shape object that contains the stated point
     * ======================================================================
     */
    @Override
    public boolean contains(int x, int y) {
        return  (Math.pow((x - this.x), 2) +
                 Math.pow((y - this.y), 2)) <=
                 Math.pow(radius, 2);
    }

    @Override
    public void draw(Graphics g) {
        g.drawOval(x - radius, y - radius, radius * 2, radius * 2);
    }
}
