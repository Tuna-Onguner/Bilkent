import java.awt.*;
import javax.swing.*;
import java.awt.event.*;
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
 * Extending 'JButton' class
 * Implementing 'ActionListener' class
 * Initializing the buttons for the binary operations
 * <======================================================>
 */
public class CalculationButton extends JButton implements ActionListener {

    //Properties
    protected Operation operation;
    protected JTextField Number01;
    protected JTextField Number02;
    protected JLabel result;
    protected CountInformer countInformer; //end properties

    //Constructor
    /**
     * <======================================================>
     * Constructor CalculationButton
     * @param operation the binary operation passed
     * <======================================================>
     */
    public CalculationButton( Operation operation ) {

        //Properties
        this.operation = operation;
        this.Number02 = new JTextField();
        this.Number01 = new JTextField();
        this.result = new JLabel("Result: ");

        //ActionListener
        this.addActionListener(this);

        //Button Style Attributes
        setPreferredSize( new Dimension( 120, 30 ) );
        setBackground( new Color( 30, 144, 255 ) ); //My favorite color ---> hex: #1E90FF
        setText( operation.getNameOfOperation() );
        setForeground( Color.WHITE );
        setBorderPainted( false );
        setVisible( true );
        setOpaque( true );
    } //end constructor

    //Methods
    /**
     * <================================================================>
     * Overriding action performed method
     * When button pressed, the desired operation is called
     * Numbers are taken from the JTextFields and result is calculated
     * Increasing the count of called by one for the applied operation
     * @param event event from actionPerformed ---> ActionListener
     * <================================================================>
     */
    @Override
    public void actionPerformed( ActionEvent event ) {
        if ( !Number01.getText().isBlank() && !Number02.getText().isBlank() ) {
            result.setText( "Result: " + ( getOperation().calculateResult
                    ( Integer.parseInt( Number01.getText().trim() ), //.trim() cleans the whitespaces
                      Integer.parseInt( Number02.getText().trim() ) ) ) );
            getOperation().setCountOfCalled( getOperation().getCountOfCalled() + 1 );
            countInformer.countUpdated();
        }
        else if ( Number01.getText().isBlank() && Number02.getText().isBlank() ||
                ( Number01.getText().isBlank() ) ) {
            JOptionPane.showMessageDialog( null, "Do not leave fields empty!",
                    "Warning", JOptionPane.WARNING_MESSAGE );
        }
    }

    /**
     * <==============================>
     * Accessor ---> getOperation
     * @return operation
     * <==============================>
     */
    public Operation getOperation() {
        return operation;
    }

    /**
     * <==============================>
     * Accessor ---> getResult
     * @return result
     * <==============================>
     */
    public JLabel getResult() {
        return result;
    } //end Methods
}
