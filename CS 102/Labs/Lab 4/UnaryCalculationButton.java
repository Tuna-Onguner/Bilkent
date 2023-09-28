import java.awt.*;
import javax.swing.*;
import java.awt.event.ActionEvent;
/**
 * <======================================================>
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * --------------------------------------------------------
 * Lab assignment - no: 4
 * CS 102, Fall 2021/2022
 * Bilkent University
 * --------------------------------------------------------
 * Class CalculationButton:
 * Non-abstract class
 * Extending 'CalculationButton' class
 * Initializing the buttons for the unary operations
 * <======================================================>
 */
public class UnaryCalculationButton extends CalculationButton {

    //Constructor
    /**
     * <=============================================>
     * Constructor UnaryCalculationButton
     * @param operation the unary operation passed
     * <=============================================>
     */
    public UnaryCalculationButton( Operation operation ) {
        super( operation );
        setBackground( Color.RED );
    } //end constructor

    //Methods
    /**
     * <===============================================================>
     * Overriding actionPerformed method for UnaryCalculationButton
     * Only one parameter, number 1 in that case, is applied
     * Setting number 2 to an empty String
     * Increasing the count of called by one for the applied operation
     * @param event event from actionPerformed ---> ActionListener
     * <===============================================================>
     */
    @Override
    public void actionPerformed( ActionEvent event ) {
        if ( !Number01.getText().isBlank() ) {
            Number02.setText("");
            result.setText("Result: " + ( getOperation().calculateResult
                    ( Integer.parseInt( Number01.getText().trim() ), 0) ) );
            getOperation().setCountOfCalled( getOperation().getCountOfCalled() + 1 );
            countInformer.countUpdated();
        }
        else if ( ( Number01.getText().isBlank() && Number02.getText().isBlank() ) ||
                  ( Number01.getText().isBlank() ) ) {
            JOptionPane.showMessageDialog( null, "Do not leave fields empty!",
                    "Warning", JOptionPane.WARNING_MESSAGE );
        }
    }//end methods
}
