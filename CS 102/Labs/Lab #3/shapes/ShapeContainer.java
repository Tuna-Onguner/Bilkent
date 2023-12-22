package shapes;
import java.util.ArrayList;
/**
 * =======================================
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * ShapeContainer Class:
 * Storing shape objects in an ArrayList
 * =======================================
 */
public class ShapeContainer {

    //Properties
    private final ArrayList<Shape> listOfShapes;

    //Constructor
    /** =====================================
     * Constructor --- ShapeContainer Class
     * Creating an ArrayList to store shapes
     * ======================================
     */
    public ShapeContainer() {
        listOfShapes = new ArrayList<>();
    }

    //Methods
    public void add(Shape s) {
        listOfShapes.add(s);
    }

    /**
     * =========================================
     * Calculating the area of the total shapes
     * @return area of the shape
     * =========================================
     */
    public double getArea() {
        double totalArea = 0;
        for (Shape shape : listOfShapes)
            totalArea = totalArea + shape.getArea();
        return totalArea;
    }

    public ArrayList<Shape> getListOfShapes() {
        return listOfShapes;
    }

    /**
     * ======================================================
     * Overriding toString Method
     * @return a proper string representation of a container
     * ======================================================
     */
    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        for (Shape shape : listOfShapes)
            stringBuilder.append(shape.toString()) ;
        return stringBuilder.toString();
    }

    /**
     * ============================================
     * Removing selected shaped from the container
     * ============================================
     */
    public void removeSelected() {
        for (int i = 0; i < listOfShapes.size(); i++) {
            if (listOfShapes.get(i) instanceof Selectable) {
                if (((Selectable) listOfShapes.get(i)).getSelected()) {
                    listOfShapes.remove(listOfShapes.get(i));
                }
            }
        }
    }

    //Two New Methods
    /**
     * ==================================
     * @return the size of the container
     * ==================================
     */
    public int size() {
        return listOfShapes.size();
    }

    /**
     * =======================================================
     * Selecting all shapes that contain stated point
     * Returning the number of shapes that contain this point
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     * @return the number of shapes which contain the point
     * =======================================================
     */
    public int selectAtAll(int x, int y) {
        int numShapesFoundAtXY = 0;
        for (Shape shape : listOfShapes) {
            if (shape instanceof Selectable) {
                if (((Selectable) shape).contains(x, y)) {
                    numShapesFoundAtXY += 1;
                    ((Selectable) shape).setSelected(true);
                }
            }
        }
        if (numShapesFoundAtXY == 1)
            numShapesFoundAtXY = 0;
        return numShapesFoundAtXY;
    }

}
