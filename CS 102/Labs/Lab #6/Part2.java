import java.util.Arrays;

/*
    MergeSort, QuickSort and Insertion Sort routines as well as the corresponding subroutines (incl. comments)
    are borrowed from geeksforgeeks
 */

public class Part2 {

    public static void main(String[] args){
        int n = 10000;
        int[] originalArray = generateRandomArray(n);
        int[] sortedArray = copyArray(originalArray);

        // You should sort the ``sortedArray`` array,
        // experiment with different sorting algorithms and measure the execution of each
        // as explained in the lab document
        // For comparison use the originalMergeSort, originalQuickSort, originalInsertionSort at the end of the code
        // Eventually, you should compare 4 methods:
        // (1) hybridSort, (2) originalMergeSort, (3) originalQuickSort, (4) originalInsertionSort
        // ...

        long startTime = System.nanoTime();
        //hybridSort(sortedArray);
        //originalMergeSort(sortedArray, 0, sortedArray.length -1);
        //originalQuickSort(sortedArray, 0, sortedArray.length -1);
        //originalInsertionSort(sortedArray);
        long endTime = System.nanoTime();
        long duration = (endTime - startTime);
        System.out.println("In milliseconds: " + (double) duration / 1000000);

        isSorted(originalArray, sortedArray);
    }
    static void hybridSort(int[] arr) {
        quickSort(arr, 0, arr.length - 1);
    }

    // Main function that sorts arr[l..r] using
    // You'll probably want to modify this function
    static void mergeSort(int[] arr, int l, int r)
    {
        if (r - l < 15) insertionSort(arr, l, r);
        if (l < r) {
            // Find the middle point
            int m =l+ (r-l)/2;

            // Sort first and second halves
            quickSort(arr, l, m);
            quickSort(arr, m + 1, r);

            // Merge the sorted halves
            merge(arr, l, m, r);
        }
    }

    // Insertion sorts given arr array
    // You'll probably want to modify this function
    static void insertionSort(int[] arr, int l, int r)
    {
        int n = arr.length;
        for (int i = l; i < r; ++i) {
            int key = arr[i];
            int j = i - 1;

            /* Move elements of arr[0..i-1], that are
               greater than key, to one position ahead
               of their current position */
            while (j >= l && arr[j] > key) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
            arr[j + 1] = key;
        }
    }

    /* The main function that implements QuickSort
          arr[] --> Array to be sorted,
          low --> Starting index,
          high --> Ending index
    */
    // You'll probably want to modify this function
    static void quickSort(int[] arr, int low, int high)
    {
        if (low < high)
        {
            if (arr.length <= 15) insertionSort(arr, low, high);
            else {
                // pi is partitioning index, arr[p]
                // is now at right place
                int pi = partition(arr, low, high);
                // Separately sort elements before
                // partition and after partition
                mergeSort(arr, low, pi - 1);
                mergeSort(arr, pi + 1, high);
            }
        }
    }

    /**
     * YOU SHOULD NOT TOUCH BELOW THIS POINT
     * YOU SHOULD NOT TOUCH BELOW THIS POINT
     * YOU SHOULD NOT TOUCH BELOW THIS POINT
     */

    // A utility function to swap two elements
    static void swap(int[] arr, int i, int j)
    {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    public static int[] generateRandomArray(int length)
    {
        int[] array = new int[length];

        for (int i = 0; i < length; ++i)
        {
            array[i] = (int)(Math.random() * length * 4);
        }

        return array;
    }

    public static int[] copyArray(int[] array)
    {
        int[] copy = new int[array.length];

        for (int i = 0; i < array.length; ++i)
        {
            copy[i] = array[i];
        }

        return copy;
    }

    public static void isSorted(int[] originalArray, int[] sortedArray)
    {
        if (originalArray.length != sortedArray.length)
            throw new RuntimeException("Array sizes are not equal");

        int[] copy = copyArray(originalArray);
        Arrays.sort(copy);

        for (int i = 0; i < originalArray.length; ++i)
        {
            if (copy[i] != sortedArray[i])
            {
                final int start = Math.max(i - 5, 0);
                final int end   = Math.min(i + 5, originalArray.length - 1);

                System.out.println("Array is not sorted!");
                System.out.printf("   %10s%10s%10s\n", "Index", "Expected", "Sorted");
                for (int j = start; j <= end; ++j)
                {
                    if (j == i)
                        System.out.print("=> ");
                    else
                        System.out.print("   ");

                    System.out.printf("%10d%10d%10d\n", j, copy[j], sortedArray[j]);
                }

                throw new RuntimeException("Array is not sorted at index " + i);
            }
        }

        System.out.println("Array is sorted!");
    }


    /* This function takes last element as pivot, places
       the pivot element at its correct position in sorted
       array, and places all smaller (smaller than pivot)
       to left of pivot and all greater elements to right
       of pivot */
    static int partition(int[] arr, int low, int high)
    {

        // pivot
        int pivot = arr[high];

        // Index of smaller element and
        // indicates the right position
        // of pivot found so far
        int i = (low - 1);

        for(int j = low; j <= high - 1; j++)
        {

            // If current element is smaller
            // than the pivot
            if (arr[j] < pivot)
            {

                // Increment index of
                // smaller element
                i++;
                swap(arr, i, j);
            }
        }
        swap(arr, i + 1, high);
        return (i + 1);
    }

    // Function to print an array
    static void printArray(int[] arr)
    {
        for (int elem : arr){
            System.out.print(elem + " ");
        }
        System.out.println();
    }

    // Merges two subarrays of arr[].
    // First subarray is arr[l..m]
    // Second subarray is arr[m+1..r]
    static void merge(int[] arr, int l, int m, int r)
    {
        // Find sizes of two subarrays to be merged
        int n1 = m - l + 1;
        int n2 = r - m;

        /* Create temp arrays */
        int[] L = new int[n1];
        int[] R = new int[n2];

        /*Copy data to temp arrays*/
        for (int i = 0; i < n1; ++i)
            L[i] = arr[l + i];
        for (int j = 0; j < n2; ++j)
            R[j] = arr[m + 1 + j];

        /* Merge the temp arrays */

        // Initial indexes of first and second subarrays
        int i = 0, j = 0;

        // Initial index of merged subarray array
        int k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        /* Copy remaining elements of L[] if any */
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        /* Copy remaining elements of R[] if any */
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    // Main function that sorts arr[l..r] using
    static void originalMergeSort(int[] arr, int l, int r)
    {
        if (l < r) {
            // Find the middle point
            int m =l+ (r-l)/2;

            // Sort first and second halves
            originalMergeSort(arr, l, m);
            originalMergeSort(arr, m + 1, r);

            // Merge the sorted halves
            merge(arr, l, m, r);
        }
    }

    // Insertion sorts given arr array
    static void originalInsertionSort(int[] arr)
    {
        int n = arr.length;
        for (int i = 1; i < n; ++i) {
            int key = arr[i];
            int j = i - 1;

            /* Move elements of arr[0..i-1], that are
               greater than key, to one position ahead
               of their current position */
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j = j - 1;
            }
            arr[j + 1] = key;
        }
    }

    /* The main function that implements QuickSort
          arr[] --> Array to be sorted,
          low --> Starting index,
          high --> Ending index
    */
    static void originalQuickSort(int[] arr, int low, int high)
    {
        if (low < high)
        {
            // pi is partitioning index, arr[p]
            // is now at right place
            int pi = partition(arr, low, high);

            // Separately sort elements before
            // partition and after partition
            originalQuickSort(arr, low, pi - 1);
            originalQuickSort(arr, pi + 1, high);
        }
    }
}
