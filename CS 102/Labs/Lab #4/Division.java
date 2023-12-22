/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Division:
 * Non-abstract class
 * Initializing the binary operation of division
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class Division extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Division:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as true, that means it is a binary operation
     * Setting the operation name of division as 'Divide'
     * <=============================================================================================>
     */
    public Division() {
        super(true, "Divide");
    } //end constructor

    //Methods
    /**
     * <======================================================>
     * calculateResult Method:
     * Division Class ---> Binary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the quotient of parameters passed
     * <======================================================>
     */
    @Override
    public int calculateResult( int a, int b ) {
        return ( a / b );
    } //end methods
}
