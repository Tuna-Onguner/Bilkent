/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Square Root:
 * Non-abstract class
 * Initializing the unary operation of square root
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class SquareRoot extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Square Root:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as false, that means it is a unary operation
     * Setting the operation name of square root as 'Square Root'
     * <=============================================================================================>
     */
    public SquareRoot() {
        super(false, "Square Root");
    } //end constructor

    //Methods
    /**
     * <===========================================================>
     * calculateResult Method:
     * Square Root Class ---> Unary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the square root of the first parameter (a) passed
     * <===========================================================>
     */
    @Override
    public int calculateResult( int a, int b ) {
        return (int) Math.sqrt( a );
    } //end methods
}
