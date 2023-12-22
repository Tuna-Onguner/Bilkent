/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Subtraction:
 * Non-abstract class
 * Initializing the binary operation of subtraction
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class Subtraction extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Subtraction:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as true, that means it is a binary operation
     * Setting the operation name of addition as 'Subtract'
     * <=============================================================================================>
     */
    public Subtraction() {
        super(true, "Subtract");
    } //end constructor

    //Methods
    /**
     * <======================================================>
     * calculateResult Method:
     * Subtraction Class ---> Binary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the difference of parameters passed
     * <======================================================>
     */
    @Override
    public int calculateResult( int a, int b )  {
        return ( a - b );
    } //end methods
}
