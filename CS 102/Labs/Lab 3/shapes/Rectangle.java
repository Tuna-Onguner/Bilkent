package shapes;
import java.awt.*;

/**
 * =======================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Rectangle Class:
 * Initialing a rectangle shape object
 * =======================================
 */
public class Rectangle extends Shape implements Selectable {

    //Properties
    private final int width, height;
    private boolean isSelected;

    //Constructor
    /**
     * ==========================================
     * Constructor --- Rectangle Class
     * Creating a rectangle shape object
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * ==========================================
     */
    public Rectangle(int width, int height) {
        super();
        this.width = width;
        this.height = height;
    }

    //Methods
    /**
     * ====================================
     * Calculating the area of a rectangle
     * @return area of the shape
     * ====================================
     */
    @Override
    double getArea() {
        return (width * height);
    }

    /**
     * ======================================================
     * Overriding toString Method
     * @return a proper string representation of a rectangle
     * ======================================================
     */
    @Override
    public String toString() {
        String isSelectedStr = "Not Selected";
        if (isSelected)
            isSelectedStr = "Selected";
        return "Rectangle (width: " + width + ", height: " + height + ") " +
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
        return (Math.abs(this.x - x) <= (width / 2)) && (Math.abs(this.y - y) <= (height / 2));
    }

    @Override
    public void draw(Graphics g) {
        g.drawRect(x, y, width, height);
    }
}
