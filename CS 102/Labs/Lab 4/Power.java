/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Root:
 * Non-abstract class
 * Initializing the binary operation of root
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class Power extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Root:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as true, that means it is a binary operation
     * Setting the operation name of root as 'Root'
     * <=============================================================================================>
     */
    public Power() {
        super(true, "Power");
    } //end constructor

    //Methods
    /**
     * <======================================================>
     * calculateResult Method:
     * Root Class ---> Binary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the cube of parameters passed
     * <======================================================>
     */
    @Override
    public int calculateResult( int a, int b ) {
        return (int) Math.pow( a, b );
    } //end methods
}

