/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Square:
 * Non-abstract class
 * Initializing the unary operation of square
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class Square extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Square:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as false, that means it is a unary operation
     * Setting the operation name of square as 'Square'
     * <=============================================================================================>
     */
    public Square() {
        super(false, "Square");
    } //end constructor

    //Methods
    /**
     * <======================================================>
     * calculateResult Method:
     * Square Class ---> Unary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the square of the first parameter (a) passed
     * <======================================================>
     */
    @Override
    public int calculateResult( int a, int b ) {
        return (int) Math.pow( a, 2 );
    } //end methods
}
