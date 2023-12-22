import java.io.*;
import java.net.URL;

/**
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * Document Class ---> Document Reader:
 * Taking text(".txt") files and processing them; placing each word into an array of term objects
 */
public class Document {

    //Expected Properties
    private String fileName;
    private Term[] listOfTerms = new Term[ 10000 ];

    //Extra Properties
    private int counter;
    private final StringBuffer stringBuffer = new StringBuffer();

    //Constructor
    /**
     * Constructor:
     * Setting the file name and starting the processing
     * @param fileName name of the file
     */
    public Document(String fileName) {
        this.fileName = fileName;
        counter = 0;
        processDocument();
    }

    //Methods
    /**
     * getFileName === Accessor
     * @return name of the file
     */
    public String getFileName() { return fileName; }

    /**
     * setFileName === Mutator
     * @param fileName name of the file
     */
    public void setFileName(String fileName) { this.fileName = fileName; }

    /**
     * getListOfTerms === Accessor
     * @return array of term objects
     */
    public Term[] getListOfTerms() { return listOfTerms; }

    /**
     * setListOfTerms === Mutator
     * @param listOfTerms array of term objects
     */
    public void setListOfTerms(Term[] listOfTerms) { this.listOfTerms = listOfTerms; }

    /**
     * processDocument === void method
     * Getting a text file into system by using the path and reading it
     * Processing the text file, placing each word into an array of term objects
     */
    public void processDocument() {
        try { //Trying to access a file...
            URL url = getClass().getResource(getFileName()); //Getting the path of the file to import it
            //FileReader fileReader = new FileReader("src/" + getFileName());   //Mac OSX--Linux directory

            FileReader fileReader = null;
            if (url != null) { //Eliminating the cases which the url is null
                fileReader = new FileReader(url.getPath()); //Importing the file into the program
            }
            else {
                System.out.println("No such file");
            }

            int reader;
            if (fileReader != null) { //Eliminating the cases which the fileReader is null
                while ((reader = fileReader.read()) != -1) { //Reading the file char by char
                    if (reader != 32) { //Taking whitespaces -- ASCII is 32
                        stringBuffer.append((char) reader); //Determining each word by using whitespaces
                    }
                    else { //When ASCII is 32, there is a new word
                        listOfTerms[ counter ] = new Term(stringBuffer.toString(). //Removing punctuation marks
                                replaceAll("\\p{Punct}", " ")); //Putting words into an array
                        stringBuffer.delete(0, stringBuffer.length()); //Cleaning StringBuffer for the next words
                        counter += 1; //Passing to an empty place in the array
                    }
                }
            }
        }
        catch (IOException exception) { //...error if file is not accessible
            System.out.println(exception.getMessage()); //Printing the error
        }
    }

    //An extra solution, works fine but not much different from the above (Do not forget to import Scanner Class)
    /*
    public void processDocumentX() {
        URL url = getClass().getResource(getFileName());
        File file = null;
        if (url != null) {
            file = new File(url.getPath());
        }
        try {
            Scanner input = new Scanner(file);
            int count = 0;
            while (input.hasNext()) {
                String word  = input.next();
                System.out.println(word);
                count += 1;
            }
        } catch (FileNotFoundException e) {
            System.out.println(e.getMessage());
        }

    }
     */

    /**
     * getCount === int method
     * @param word word, which is entered by the user in order to search
     * @return count the number of a particular word in the document
     */
    public int getCount(String word) {
        int count = 0;
        for (Term listOfTerm : listOfTerms) { //Counting each word from the array of terms
            if (listOfTerm != null) { //Passing nulls
                if (word.equalsIgnoreCase(listOfTerm.getWord()))
                    count += 1;
            }
        }
        return count;
    }

    /**
     * getFrequency === double method
     * @param word word, which is entered by the user in order to search
     * @return frequency of the entered word ---> [(number of the word) / (total words)]
     */
    public double getFrequency(String word) {
        double totalWords = 0;
        for (Term listOfTerm : listOfTerms) {
            if (listOfTerm != null) //Passing nulls
                totalWords += 1;
        }
        return (getCount(word) /totalWords);
    }
}
