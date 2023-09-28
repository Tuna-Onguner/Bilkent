import javax.swing.*;
import java.awt.*;
/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class CalculatorPanel:
 * Non-abstract class
 * Initializing the panel for the calculator app
 * Extending 'JPanel' class
 * Initialing the GUI components including buttons
 * <======================================================>
 */
public class CalculatorPanel extends JPanel implements CountInformer {

    //Properties
    JLabel labelNumber_1;
    JLabel labelNumber_2;
    JLabel massage1;
    JLabel massage2;
    JTextField number1;
    JTextField number2;
    JLabel result;

    //Array of operation objects
    Operation[] operations;

    //Binary Operations
    Operation power;
    Operation addition;
    Operation division;
    Operation subtraction;
    Operation multiplication;

    //Unary Operations
    Operation cube;
    Operation square;
    Operation factorial;
    Operation logarithm;
    Operation squareRoot;

    //Button
    CalculationButton button;

    //Layout
    JPanel cardsLay1;
    JPanel cardsLay2;
    JPanel cardsLay3;
    JPanel cardsLay4;
    JPanel card1;
    JPanel card2;

    Dimension dim = new Dimension( 680, 40 );

    public CalculatorPanel() {

        //Properties
        labelNumber_1 = new JLabel(" Number 1:");
        labelNumber_2 = new JLabel(" Number 2:");
        number1 = new JTextField();
        number2 = new JTextField();
        result = new JLabel();

        //Array of operation objects
        operations = new Operation[ 10 ];

        //Binary Operations
        power = new Power();
        addition = new Addition();
        division = new Division();
        subtraction = new Subtraction();
        multiplication = new Multiplication();

        //Unary Operations
        cube = new Cube();
        square = new Square();
        factorial = new Factorial();
        logarithm = new Logarithm();
        squareRoot = new SquareRoot();

        //Binary Operations into array
        operations[ 0 ] = addition;
        operations[ 1 ] = subtraction;
        operations[ 2 ] = multiplication;
        operations[ 3 ] = division;
        operations[ 4 ] = power;

        //Unary Operations into array
        operations[ 5 ] = square;
        operations[ 6 ] = squareRoot;
        operations[ 7 ] = cube;
        operations[ 8 ] = factorial;
        operations[ 9 ] = logarithm;

        //Layout
        cardsLay1 = new JPanel( new CardLayout() );
        cardsLay1.setPreferredSize( dim );
        cardsLay2 = new JPanel( new CardLayout() );
        cardsLay2.setPreferredSize( dim );
        cardsLay3 = new JPanel( new CardLayout() );
        cardsLay3.setPreferredSize( dim );
        cardsLay4 = new JPanel( new CardLayout() );
        cardsLay4.setPreferredSize( dim );
        card1 = new JPanel();
        card2 = new JPanel();

        //Massages
        massage1 = new JLabel(
                "Add: " + addition.getCountOfCalled() + "\n".repeat(24) +
                     "Subtract: " + subtraction.getCountOfCalled() + "\n".repeat(26) +
                     "Multiply: " + multiplication.getCountOfCalled() + "\n".repeat(15) +
                     "Divide: " + division.getCountOfCalled() + "\n".repeat(24) +
                     "Power: " + power.getCountOfCalled() +"\n" );

        massage2 = new JLabel(
                 "Square: " + square.getCountOfCalled() + "\n".repeat(20) +
                      "Square Root: " + squareRoot.getCountOfCalled() + "\n".repeat(20) +
                      "Cube: " + cube.getCountOfCalled() + "\n".repeat(20) +
                      "Factorial: " + factorial.getCountOfCalled() + "\n".repeat(20) +
                      logarithm.getNameOfOperation() + ": " + logarithm.getCountOfCalled()
        );

        massage1.setPreferredSize( new Dimension( 680, 40 ) );
        massage2.setPreferredSize( new Dimension( 680, 40 ) );

        //JTextFields
        number1.setPreferredSize( new Dimension( 130, 30 ) );
        number2.setPreferredSize( new Dimension( 130, 30 ) );

        //Buttons
        for ( Operation operation : operations ) {

            if ( operation.isUnaryOrBinary() ) {

                button = new CalculationButton( operation );
                button.Number01 = number1;
                button.Number02 = number2;
                button.result = result;
                button.countInformer = this;
                card1.add( button );
                cardsLay1.add( card1, null );
            }
            else {

                button = new UnaryCalculationButton( operation );
                button.Number01 = number1;
                button.Number02 = number2;
                button.result = result;
                button.countInformer = this;
                card2.add( button );
                cardsLay2.add( card2, null );
            }
        }

        //Adding components into the panel
        this.add( this.labelNumber_1 );
        this.add( this.number1 );
        this.add( this.labelNumber_2 );
        this.add( this.number2 );
        this.add( this.result );
        this.add( this.cardsLay1 );
        this.add( this.cardsLay2 );
        this.add( this.massage1 );
        this.add( this.massage2 );
    }

    //Methods
    /**
     * <============================================================>
     * calculateAndUpdateCountMessage method:
     * Updating the theCountOfCalled instant of the operation
     * Invoked when one of the buttons is pressed
     * Invoked by countUpdated methods of CountInformer Interface
     * Putting enough spaces between components for a good UI
     * <============================================================>
     */
    public void calculateAndUpdateCountMessage() {
        massage1.setText(
                "Add: " + addition.getCountOfCalled() + "\n".repeat(24) +
                        "Subtract: " + subtraction.getCountOfCalled() + "\n".repeat(26) +
                        "Multiply: " + multiplication.getCountOfCalled() + "\n".repeat(15) +
                        "Divide: " + division.getCountOfCalled() + "\n".repeat(24) +
                        "Power: " + power.getCountOfCalled() +"\n" );

        massage2.setText(
                "Square: " + square.getCountOfCalled() + "\n".repeat(20) +
                        "Square Root: " + squareRoot.getCountOfCalled() + "\n".repeat(20) +
                        "Cube: " + cube.getCountOfCalled() + "\n".repeat(20) +
                        "Factorial: " + factorial.getCountOfCalled() + "\n".repeat(20) +
                        logarithm.getNameOfOperation() + ": " + logarithm.getCountOfCalled()
        );
    }

    /**
     * <================================================================>
     * countUpdate method ---> implemented from CountInformer Interface
     * Invokes calculateAndUpdateCountMessage when it is called
     * It is called when any button is pressed
     * <================================================================>
     */
    @Override
    public void countUpdated() {
        calculateAndUpdateCountMessage();
    } //end methods
}
