/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class Operation:
 * Public and Abstract class
 * Initializing the all operations that are defined
 * Having calculateResult method, accessors, and mutators
 * <======================================================>
 */
public abstract class Operation {

    //Properties
    private int countOfCalled;
    private final boolean unaryOrBinary; //'true' for binary operations and 'false' for unary operations
    private final String nameOfOperation; //end properties

    //Constructor
    /**
     * <==========================================================================>
     * Constructor of Operation:
     * Initializing the operation as an object
     * Taking two parameters
     * @param unaryOrBinary a boolean whether the operation is binary or unary;
     *                      true for binary operations,
     *                      false for unary operations
     * @param nameOfOperation the chosen name of the operation
     * <==========================================================================>
     */
    public Operation( boolean unaryOrBinary, String nameOfOperation ) {
        this.countOfCalled = 0;
        this.unaryOrBinary = unaryOrBinary;
        this.nameOfOperation = nameOfOperation;
    } //end constructor

    //Methods
    /**
     * <=======================================================================>
     * calculateResult Method:
     * Returning the result of calculation respect to the type of operation
     * @param a the first parameter passed by the user
     * @param b the second parameter passed by the user
     * @return the final result according to the operation
     * <=======================================================================>
     */
    public abstract int calculateResult( int a, int b ); //end methods

    //Accessors
    /**
     * <==============================>
     * Accessor ---> countOfCalled
     * @return countOfCalled
     * <==============================>
     */
    public int getCountOfCalled() {
        return countOfCalled;
    }

    /**
     * <==============================>
     * Accessor ---> unaryOrBinary
     * @return unaryOrBinary
     * <==============================>
     */
    public boolean isUnaryOrBinary() {
        return unaryOrBinary;
    }

    /**
     * <==============================>
     * Accessor ---> nameOfOperation
     * @return nameOfOperation
     * <==============================>
     */
    public String getNameOfOperation() {
        return nameOfOperation;
    } //end accessors

    //Mutators
    /**
     * <====================================>
     * Mutator ---> countOfCalled
     * @param countOfCalled countOfCalled
     * <====================================>
     */
    public void setCountOfCalled( int countOfCalled ) {
        this.countOfCalled = countOfCalled;
    }
}
