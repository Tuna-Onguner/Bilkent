/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Factorial:
 * Non-abstract class
 * Initializing the unary operation of factorial
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class Factorial extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Factorial:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as false, that means it is a unary operation
     * Setting the operation name of factorial as 'Factorial'
     * <=============================================================================================>
     */
    public Factorial() {
        super(false, "Factorial");
    } //end constructor

    //Methods
    /**
     * <==========================================================>
     * calculateResult Method:
     * Factorial Class ---> Unary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the factorial of the first parameter (a) passed
     * <==========================================================>
     */
    @Override
    public int calculateResult( int a, int b ) {
        int factorial = 1;
        for ( int i = 1; i <= a; i++ )
            factorial *= i;
        return factorial;
    } //end methods
}
