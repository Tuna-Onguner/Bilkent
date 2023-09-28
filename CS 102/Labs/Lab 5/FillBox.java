import java.util.Arrays;
import java.util.Scanner;

/**
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 *
 * Date: December of 1, 2021, Wednesday
 * CS 102, Lab 05, Fill The Box
 * Bilkent University
 *
 * Main Class ---> FillBox:
 * The algorithm takes values from the user: number of rows, columns, and a character
 * A rectangle is created according to the entered values
 * The rectangle is filled by entered character
 * Then, the rectangle is divided into smaller rectangles
 * If the sizes of rectangle are smaller than seven, the rectangle cannot be divided further
 * Finally, the divided rectangles are also filled by the entered character
 */
public class FillBox {

    public static char Char;

    /**
     * main Method:
     * Takes inputs from the user in order to initialize a valid rectangle
     * Absolute values of the inputs are taken to avoid negative input related errors
     * @param args args of the main method
     */
    public static void main(String[] args) {

        Scanner rowScanner = new Scanner(System.in);
        System.out.print("Rows: ");
        int row = Math.abs(rowScanner.nextInt());

        Scanner columnScanner = new Scanner(System.in);
        System.out.print("Columns: ");
        int column = Math.abs(columnScanner.nextInt());

        Scanner charScanner = new Scanner(System.in);
        System.out.print("Char: ");
        char ch = charScanner.next().charAt(0);
        Char = ch;

        char[][] array = createArray(row, column);
        printArray(array);
        System.out.println();
        printArray(fillBox(array, row, column, ch));
        System.out.println();
        char[][] array2 = createArray(row, column);
        drawFigure(array2, 0, 0, array.length);
        printArray(array2);
    }

    /**
     * createArray Method:
     * Initializing a valid rectangle
     * @param row row
     * @param column column
     * @return a rectangle
     */
    public static char[][] createArray(int row, int column) {

        char[][] array = new char[row][column];

        for (int i = 0; i < array.length; i++) {

            array[i][0] = '#';
            array[i][column - 1] = '#';

            for (int j = 1; j < array[0].length - 1; j++) {
                array[i][j] = ' ';
                array[i][column  - 2] = ' ';
            }
        }

        Arrays.fill(array[0], '#');
        Arrays.fill(array[row - 1], '#');

        return array;
    }

    /**
     * printArray Method:
     * Printing the array as a rectangle
     * @param array an array rectangle
     */
    public static void printArray(char[][] array) {

        for (char[] chars : array) System.out.println(chars);
    }

    /**
     * fillBox Method:
     * Filling the array rectangle
     * @param array array rectangle
     * @param row row
     * @param column column
     * @param ch character
     * @return filled rectangle
     */
    public static char[][] fillBox(char[][] array, int row, int column, char ch) {

            if (array[row - 2][column - 2] == ' ') {
                array[row - 2][column - 2] = ch;
                fillBox(array, row, column - 1, ch);
                fillBox(array, row - 1, column, ch);
            }
        return array;
    }

    public static void drawFigure(char[][] square, int x, int y, int size) {

        if (size < 7) {
            fillBox(square, x + size + 1, y + size + 1, Char);
        }
        else {
            for (int i = 0; i < size; i++) {

                if (square[x + size / 2][i + y] != '#') {
                    square[x + size / 2][i + y] = '#';
                }
                if (square[i + x][y + size / 2] != '#') {
                    square[i + x][y + size / 2] = '#';
                }
            }
                drawFigure(square, x, y, size / 2);
                drawFigure(square, x + size / 2, y, size / 2);
                drawFigure(square, x, y + size / 2, size / 2);
                drawFigure(square, x + size / 2, y + size / 2, size / 2);
        }
    }
}
