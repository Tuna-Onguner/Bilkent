import java.util.Random;

/**
 * @author Deniz Tuna Onguner - 22001788
 * CS 102, Fall 2021
 *
 * Hangman Class
 */
public class Hangman {

    //PROPERTIES
    private StringBuffer secretWord = new StringBuffer(); //StringBuffer properties
    private StringBuffer usedLetters = new StringBuffer(); //StringBuffer properties
    private StringBuffer knownSoFar = new StringBuffer(); //StringBuffer properties
    private final StringBuffer allLetters = new StringBuffer("ABCDEFGHIJKLMNOPQRSTUVWXYZ"); //the full English alphabet
    private int numberOfIncorrectTries, maxAllowedIncorrectTries; //integer properties

    //CONSTRUCTOR
    /**
     * Constructor Hangman:
     * Sets the "MaxAllowedIncorrectTries" and "SecretWord"
     */
    public Hangman() {

        setNumberOfIncorrectTries(0);
        setMaxAllowedIncorrectTries(6); //setting max allowed incorrect tries to 6
        setSecretWord(new StringBuffer(chooseSecretWord())); //choosing a secret word randomly from the list
    }

    //ACCESSORS
    /**
     * Accessor - getAllLetters():
     * @return all letters in English alphabet
     */
    public StringBuffer getAllLetters() { return allLetters; }

    /**
     * Accessor - getSecretWord():
     * @return the secret word
     */
    public StringBuffer getSecretWord() { return secretWord; }

    /**
     * Accessor - getUsedLetters():
     * @return the used letters
     */
    public StringBuffer getUsedLetters() { return usedLetters; }

    /**
     * Accessor - getKnownSoFar():
     * @return the letters known up until that point
     */
    public StringBuffer getKnownSoFar() { return knownSoFar; }

    /**
     * Accessor - getMaxAllowedIncorrectTries():
     * @return max allowed Incorrect attempt, which is six
     */
    public int getMaxAllowedIncorrectTries() { return maxAllowedIncorrectTries; }

    /**
     * Accessor - getNumberOfIncorrectTries():
     * @return number of attempts of incorrect tries up until that point
     */
    public int getNumberOfIncorrectTries() { return numberOfIncorrectTries; }

    //MUTATORS
    /**
     * Mutator - setSecretWord():
     * @param secretWord the word chosen from the fixed word list randomly
     */
    public void setSecretWord(StringBuffer secretWord) { this.secretWord = secretWord; }

    /**
     * Mutator - setUsedLetters():
     * @param usedLetters the used letters
     */
    public void setUsedLetters(StringBuffer usedLetters) { this.usedLetters = usedLetters; }

    /**
     * Mutator - setKnownSoFar():
     * @param knownSoFar the letters known up until that point
     */
    public void setKnownSoFar(StringBuffer knownSoFar) { this.knownSoFar = knownSoFar; }

    /**
     * Mutator - setMaxAllowedIncorrectTries():
     * @param maxAllowedIncorrectTries max allowed Incorrect attempt, which is six
     */
    public void setMaxAllowedIncorrectTries(int maxAllowedIncorrectTries) {
        this.maxAllowedIncorrectTries = maxAllowedIncorrectTries;
    }

    /**
     * Mutator - setNumberOfIncorrectTries():
     * @param numberOfIncorrectTries number of attempts of incorrect tries up until that point
     */
    public void setNumberOfIncorrectTries(int numberOfIncorrectTries) {
        this.numberOfIncorrectTries = numberOfIncorrectTries;
    }

    //OTHER METHODS
    /**
     * Method - chooseSecretWord():
     * @return a String that is the secret word
     */
    private String chooseSecretWord() {

        //the fixed word list with 39 String elements that are almost all associated with programming somehow
        String[] fixedWordList = new String[] {"JAVA", "PYTHON", "LINUX", "UNIX", "MACOS", "INTERFACE",
                "WINDOWS", "BILKENT", "COMPUTER", "JAVASCRIPT", "INHERITANCE", "PROGRAMMING", "ALGORITHM",
                "ARRAY", "AUTONOMOUS", "CODING", "INTELLIJ", "STACKOVERFLOW", "ANGULAR", "BOOLEAN", "COMPILER",
                "DATABASE", "DJANGO", "GITHUB", "SWIFT", "GOLANG", "JQUERY", "MANGODB", "PARAMETER", "POINTER",
                "RUBY", "REACT", "TERMINAL", "XCODE", "BOOTCAMP", "FRAMEWORK", "BINARY", "ENGINEER", "INTERNET"};
        int random = new Random().nextInt(fixedWordList.length); //random class to choose an element randomly
        return fixedWordList[random]; //returning the chosen element
    }

    /**
     * Method - tryThis():
     * @param letter the letter which is entered by the user
     * @return the number of letters in the word
     */
    public int tryThis(char letter) {

        int counterOfLetters = 0; //a local variable
        for (int i = 0; i <= secretWord.length() - 1; i++) {

            if (secretWord.charAt(i) == letter) { //counting the letters

                counterOfLetters++;
            }
        }
        return counterOfLetters; //returning the final count
    }

    /**
     * Method - isGameOver():
     * @return whether the game is over or not
     */
    public boolean isGameOver() {
        return knownSoFar == secretWord || hasLost();
    }

    /**
     * Method - hasLost():
     * @return whether the player has lost or not
     */
    public boolean hasLost() {
        return numberOfIncorrectTries >= maxAllowedIncorrectTries;
    }

    /**
     * Method - displaySecretWordAsHidden():
     * @param letter is entered by the user
     */
    public void displaySecretWordAsHidden(char letter, StringBuffer secretWordAsHidden) {

        for (int j = 0; j < secretWord.length(); j++) {

            if (secretWord.charAt(j) == letter) {

                    secretWordAsHidden.replace(j, j + 1, Character.toString(letter));
            }
        }
        System.out.println(secretWordAsHidden);
    }

}
