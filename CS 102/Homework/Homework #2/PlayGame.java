import java.util.Scanner;

/**
 * @author Deniz Tuna Onguner - 22001788
 * CS 102, Fall 2021
 *
 * PlayGame Class
 */
public class PlayGame {

    public static void main(String[] args) {

        do {
            Hangman hangman = new Hangman(); //creating a Hangman Object and starting the game

            //building a string that is a displaying of the secret word as the characters are hidden till they're founded
            System.out.println("\n" + "_".repeat(hangman.getSecretWord().length())); //displaying the hidden characters
            StringBuffer secretWordAsHidden = new StringBuffer("_".repeat(hangman.getSecretWord().length()));

            while (!hangman.isGameOver()) { //loop restarts the game until the user wants to stop playing

                System.out.print("Guess a letter or the word: "); //asking the user make a letter or a word guess
                Scanner input = new Scanner(System.in); //taking the answer of the user into system

                String inputToString = input.next().toUpperCase(); //solving uppercase-lowercase related issues

                if (inputToString.equalsIgnoreCase(hangman.getSecretWord().toString())) { //if the user guess the word correctly
                    System.out.println("That is correct. Congratulations!"); //congratulating the player
                    break;
                }

                char inputToChar = inputToString.charAt(0); //String to char
                if (inputToChar >= 'A' && inputToChar <= 'Z') { //if input is alphabetical or not

                    if (hangman.tryThis(inputToChar) != 0) { //checking whether the input is a correct one

                        hangman.setKnownSoFar(hangman.getKnownSoFar().append(inputToChar)); //updating known letter list
                    }
                    else if(!hangman.getUsedLetters().toString().contains(Character.toString(inputToChar))) {
                        //if the input is incorrect, updating all related variables and objects
                        hangman.setNumberOfIncorrectTries(hangman.getNumberOfIncorrectTries() + 1);
                        hangman.setUsedLetters(hangman.getUsedLetters().append(inputToChar));
                    }
                    System.out.println("Used Letters: " + hangman.getUsedLetters()); //displaying the used letters list
                    System.out.println("Known So Far: " + hangman.getKnownSoFar()); //displaying the known letters list
                    System.out.println("Incorrect Tries: " + hangman.getNumberOfIncorrectTries() + "\n");
                    if (hangman.isGameOver()) { //whether the game is over
                        //feeling bad for the player to lose
                        System.out.println("Oops, you lose. It was '" + hangman.getSecretWord() + "'. Try Again.");
                        break;
                    }

                } else {

                    //warning the user to enter only logical things
                    System.out.println("Please, enter only alphabetical characters!");
                }
                hangman.displaySecretWordAsHidden(inputToChar, secretWordAsHidden); //displaying the hidden secret word

            }

        }
        while (playAgain()); //loop to continue starting new games as long as the user wants to continue
        System.out.println("Thanks for playing \u2764\uFE0F"); //thanks the user to play with a smiling emoji
    }

    /**
     * Method - PlayAgain():
     * @return true if the user wants to continue playing, otherwise false
     */
    public static boolean playAgain() {

        System.out.print( "\n" + "Do you wanna play again? (y/n): " ); //yes or no
        Scanner input = new Scanner( System.in );
        String isEndGame = input.nextLine();
        return isEndGame.equalsIgnoreCase("y"); //returning true if yes
    }

}
