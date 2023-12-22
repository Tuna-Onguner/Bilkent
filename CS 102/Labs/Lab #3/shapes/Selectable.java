package shapes;
/**
 * ==========================================================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Selectable Inheritance:
 * Making shapes selectable according to their location and the stated point
 * ==========================================================================
 */
public interface Selectable {

    //Methods
    /**
     * =========================================================================
     * Getting the selected situation of a shape ("selected" or "not selected")
     * @return selected situation
     * =========================================================================
     */
    boolean getSelected();

    /**
     * ============================================================================
     * Setting the selected situation of a shape according to the stated parameter
     * @param bln the new situation of the shape object
     * ============================================================================
     */
    void setSelected(boolean bln);

    /**
     * =====================================================================
     * Creating and returning to a new shape that contains the stated point
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     * @return a new shape object that contains the stated point
     * ======================================================================
     */
    boolean contains(int x, int y);
}
