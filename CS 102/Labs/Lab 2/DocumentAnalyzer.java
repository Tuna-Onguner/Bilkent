import java.util.Scanner;

/**
 * @author Deniz Tuna Onguner - 22001788
 * @version 1.0
 * DocumentAnalyzer Class ---> Document Reader:
 * Main Class + Test Class
 */
public class DocumentAnalyzer {
    public static int numberOfTotalDocuments;

    /**
     * main method
     * Displaying the menu, taking the options and file names
     * Starting the processing document and printing the expected results
     * @param args args
     */
    public static void main(String[] args) {
        //First Step of the Menu
        System.out.print("Enter the number of documents: ");
        Scanner input = new Scanner(System.in); //Taking the total number of files
        numberOfTotalDocuments = input.nextInt();
        Document[] documents = new Document[ numberOfTotalDocuments ]; //Creating an array to store the files

        //Second Step of the Menu
        int j = 1;
        do {
            System.out.println("Enter the name of the document " + j + ": ");
            Scanner doc = new Scanner(System.in); //Taking the name of the documents
            documents[ j - 1 ] = new Document(doc.nextLine()); //Filling array of documents
            j++;
        }
        while (j <= numberOfTotalDocuments);

        //Third Step of the Menu
        String optionString;
        int optionInteger;
        do { //Printing the menu until the user wants to exit/quit
            System.out.println("Enter an option (enter x to quit): ");
            Scanner optionScanner = new Scanner(System.in); //Taking the option
            optionString = optionScanner.nextLine();
            if (!optionString.equalsIgnoreCase("x"))
                optionInteger = Integer.parseInt(optionString);
            else
                break;

            if (optionInteger == 3) { //Option 3 --- tf-idf
                System.out.println("Enter the word which you want to calculate tf-idf: ");
                Scanner wordScan = new Scanner(System.in);
                String word = wordScan.nextLine();
                calcTfIdf(documents, word);
            }
            else if (optionInteger == 2) { //Option 2 --- most repeated word's number of appearance
                for (int i = 1; i <= numberOfTotalDocuments; i++) {
                    String mostRepeatedWord = "";
                    int count, maximum = 0, numberOfAppearance = 0;
                    for(int d = 0; d < documents[ i - 1 ].getListOfTerms().length; d++){
                        count = 1;
                        for(int k = d + 1; k < documents[ i - 1 ].getListOfTerms().length; k++){
                            if (documents[ i - 1 ].getListOfTerms()[ k ] != null) {
                                if (documents[ i - 1 ].getListOfTerms()[ d ].getWord().equals
                                        (documents[ i - 1 ].getListOfTerms()[ k ].getWord())) {
                                    count += 1;
                                }
                            }
                        }
                        if(count > maximum){
                            maximum = count;
                            mostRepeatedWord = documents[ i - 1 ].getListOfTerms()[ d ] .getWord();
                            numberOfAppearance = documents[ i - 1 ].getCount(mostRepeatedWord);
                        }
                    }
                    System.out.println("Document " + i + "\n\tword: " + mostRepeatedWord +
                            "\n\tnumber of appearance: " + numberOfAppearance);
                }

            }
            else if (optionInteger == 1) { //Option 1 --- frequency
                System.out.println("Enter the word which you want to find the frequency: ");
                Scanner wordScan = new Scanner(System.in);
                String word = wordScan.nextLine();
                for (int i = 1; i <= numberOfTotalDocuments; i++) {
                    System.out.println("Document " + i + "\n\tword: " + word +
                            "\n\tterm frequency: " + documents[ i - 1 ].getFrequency(word));
                }
            }
            else
                System.out.println("No such command, try something else"); //Non-existing option

        }
        while (!optionString.equalsIgnoreCase("x"));

    }

    /**
     * calcTfIdf method
     * @param documents an array of document objects entered by the user
     * @param word word that is entered by the user
     */
    public static void calcTfIdf(Document[] documents, String word) {
        double tf;
        double idf;
        int docNum = 1;

        for (Document document : documents) {
            tf = document.getFrequency(word);
            idf = Math.log10((double) numberOfTotalDocuments / numberOfDocumentsIncludingT(documents, word));
            System.out.println("Document " + docNum + "\n\tword: " + word + "\n\ttf-idf: " + (idf * tf));
            docNum += 1;
        }
    }

    /**
     * numberOfDocumentsIncludingT method:
     * Taking the number of documents which contains the word entered by the user
     * @param documents an array of document objects entered by the user
     * @param word word that is entered by the user
     * @return number of documents that includes the defined word
     */
    public static int numberOfDocumentsIncludingT(Document[] documents, String word) {
        int numberOfDocumentsIncludingT = 0;

        for (Document document : documents) {
            for (Term term : document.getListOfTerms()) {
                if (term != null) {
                    if (term.getWord().equalsIgnoreCase(word)) {
                        numberOfDocumentsIncludingT += 1;
                        break;
                    }
                }
                else
                    break;
            }
        }
        return numberOfDocumentsIncludingT;
    }
}
