/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Logarithm:
 * Non-abstract class
 * Initializing the unary operation of logarithm
 * Extending 'Operation' class
 * Overriding calculateResult method of Operation class
 * <======================================================>
 */
public class Logarithm extends Operation {

    //Constructor
    /**
     * <=============================================================================================>
     * Constructor of Logarithm:
     * Not taking any parameters
     * Setting the boolean variable of Operation class as false, that means it is a unary operation
     * Setting the operation name of logarithm as 'Log10'
     * <=============================================================================================>
     */
    public Logarithm() {
        super(false, "log\u2081\u2080");
    } //end constructor

    //Methods
    /**
     * <==============================================================>
     * calculateResult Method:
     * Logarithm Class ---> Unary Operation
     * Overriding calculateResult method of Operation class
     * @param a the first parameter passed
     * @param b the second parameter passed
     * @return the logarithm 10 of the first parameter (a) passed
     * <==============================================================>
     */
    @Override
    public int calculateResult( int a, int b ) {
        return (int) Math.log10( a );
    } //end methods
}
