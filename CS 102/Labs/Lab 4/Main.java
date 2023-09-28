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
 * Main Class ---> Calculator App
 * <======================================================>
 */
public class Main {

    //Main Method
    public static void main(String[] args) {

        //Properties
        JFrame frame = new JFrame( "Bilculator" );
        CalculatorPanel panel = new CalculatorPanel(); //end properties

        frame.add( panel );
        frame.setVisible( true );
        frame.setResizable( false );
        frame.setBackground( Color.BLUE );
        frame.setSize( 780, 280 );
        frame.setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
    } //end main method
}
