package shapes;
/**
 * =============================================================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Locatable Inheritance:
 * Making shapes locatable by setting and getting their coordinates
 * =============================================================================
 */
public interface Locatable {

    //Methods
    /**
     * ====================================
     * Getting the x-coordinate of a shape
     * @return x-coordinate of the shape
     * ====================================
     */
    int getX();

    /**
     * ====================================
     * Getting the y-coordinate of a shape
     * @return y-coordinate of the shape
     * ====================================
     */
    int getY();

    /**
     * ======================================================================
     * Locating a shape into a position according to the stated coordinates
     * @param x stated x-coordinate
     * @param y stated y-coordinate
     * ======================================================================
     */
    void setLocation(int x, int y);
}
