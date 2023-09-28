/**
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Term Class ---> Document Reader:
 * Initializing term objects, which takes a word as parameter
 */
public class Term {

    //Properties
    private String word;
    private int count;

    //Constructor
    /**
     * Constructor:
     * Setting the String value (word) of the object
     * @param word String value of the term object
     */
    public Term(String word) {
        this.word = word;
        this.count = 0;
    }

    //Methods
    /**
     * getWord === Accessor
     * @return word entered by the user
     */
    public String getWord() {
        return word;
    }

    /**
     * setWord === Mutator
     * @param word word, entered by the user
     */
    public void setWord(String word) {
        this.word = word;
    }

    /**
     * @return count of the word in the document
     */
    public int getCount() {
        return count;
    }

    /**
     * @param count count of the word in the document
     */
    public void setCount(int count) {
        this.count = count;
    }

    /**
     * incrementCount === void method
     * Increasing the number of the word one when each time it is called
     */
    public void incrementCount() {
        count += 1;
    }

    /**
     * Overriding the toString Method, just for testing whether the class work as it's expected, or not
     * @return word, that is stored by the term object
     */
    @Override
    public String toString() {
        return word;
    }
}
