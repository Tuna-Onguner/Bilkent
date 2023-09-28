import java.util.Scanner;

/**
 * Test Class ---> Monte Carlo Simulation
 * PiCalculator the test class
 * CS 102, Fall 2021/2022
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 */
public class PiCalculator {

    public static void main(String[] args) {

        Rectangle rectangle = new Rectangle(10,10, new Point(0,0));
        Circle circle = new Circle(5, new Point(5,5));

        System.out.print("Enter a number to approach to \u03C0 as much as possible: ");
        Scanner userInput = new Scanner(System.in);

        double userValue = userInput.nextDouble();
        double tries = 0;
        double hit = 0;

        for ( int counter = 0; counter <= userValue; counter++ ) {
            Point point = rectangle.getRandomPoints();
            tries += 1;
            if (circle.contains(point))
                hit += 1;
        }
        System.out.println("Number of tries: " + (int)tries + "\nNumber of hits: " +
                          (int)hit + "\n\u03C0 \u2248 " + ((double) 4 * hit /tries));
    }
}
