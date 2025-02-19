/*!
 * \par Bilkent University
 * \par Spring 2025
 *
 * \par Department:
 * Computer Engineering
 *
 * \par Course:
 * CS481 Bioinformatics Algorithms
 *
 * \par Assignment:
 * Homework Assignment #0
 *
 * \par Section:
 * 1
 *
 * \par Instructor:
 * Can Alkan
 *
 * \author Deniz Tuna ONGUNER (22001788)
 * \date 15.02.2025 (DD/MM/YYYY)
 *
 * \file main.c
 * \brief A program that generates random square matrices and performs addition,
 * scalar multiplication, and transposition operations on them.
 *
 * \par C Standard:
 * GNU99
 *
 * \details The program takes two arguments:
 * - `-n` (non-negative int): size of the square matrices to be generated
 * - `-s` (int): scalar value for scalar multiplication
 */

#include <time.h>   // For time(NULL) function
#include <stdio.h>  // For printf and fprintf functions
#include <stdlib.h> // For malloc, free, and rand functions
#include <unistd.h> // For getopt function

/**
 * @def CONV_BASE
 * @brief The base used for converting strings to integers with `strtol` function.
 */
#define CONV_BASE 10

/**
 * @def USAGE_ERROR
 * @brief Error message displayed when incorrect arguments are provided.
 */
#define USAGE_ERROR "Usage: %s -n <non-negative integer> -s <integer>\n\tTerminating...\n"

/**
 * @struct MatrixNode
 * @brief Represents a node in a doubly linked list for a square matrix.
 *
 * Each node stores a value along with its row and column indices in the matrix.
 * The nodes are linked in a doubly circular linked list.
 */
typedef struct MatrixNode {
    int value; /**< @brief The value of the matrix element. */
    int row_index; /**< @brief The row index of the matrix element. */
    int col_index; /**< @brief The column index of the matrix element. */
    struct MatrixNode *next; /**< @brief Pointer to the next node in the linked list. */
    struct MatrixNode *prev; /**< @brief Pointer to the previous node in the linked list. */
} MatrixNode;

// Function prototypes
// --------------------------------------------------------------------------
// Find the detailed explanation of the functions in the function definitions
MatrixNode *create_node(int, int, int);

void insert_node(MatrixNode **, int, int, int);

void generate_randomly_filled_smatrix(MatrixNode **, int);

int get_smatrix_entry(const MatrixNode *, int, int);

void add_two_smatrices(MatrixNode **, MatrixNode **, MatrixNode **, int);

void smatrix_scalar_multiplication(MatrixNode **, MatrixNode **, int, int);

void transpose_smatrix(MatrixNode **, MatrixNode **, int);

void print_smatrix(MatrixNode **, int);

void free_smatrix(MatrixNode **);

// --------------------------------------------------------------------------

/**
 * @brief The main function of the program
 * @param argc Number of arguments, 5 is expected
 * @param argv Command line arguments
 * @return 0 if the program runs successfully, 1 otherwise
 */
int main(const int argc, char **argv) {
    if (argc != 5) {
        /*
         * The program must be called with exactly 5 arguments:
         * 1: Executable name, i.e. hw0
         * 2: -n flag
         * 3: Size of the square matrix
         * 4: -s flag
         * 5: Scalar value
         */
        fprintf(stderr, USAGE_ERROR, argv[0]);
        return 1;
    }

    int smatrix_size = 0; // The word "smatrix" is used to represent "square matrix"
    int scalar_value = 0; // Scalar value is default 0

    // Parse command line arguments
    int opt;
    char *chr_ptr;
    while ((opt = getopt(argc, argv, "n:s:")) != -1) {
        switch (opt) {
            case 'n':
                smatrix_size = (int) strtol(optarg, &chr_ptr, CONV_BASE);
                // first argument, optarg, is the string to be converted to an integer
                if (*chr_ptr != '\0') {
                    // The string contains non-numeric characters
                    fprintf(stderr, "Matrix size must be a non-negative integer\n\tTerminating...\n");
                    return 1;
                }
                // second argument, chrptr, is the pointer to the first invalid character in the string
                // third argument, CONV_BASE, is the base of the number system; 10 for decimal
                break;
            case 's':
                scalar_value = (int) strtol(optarg, &chr_ptr, CONV_BASE);
                if (*chr_ptr != '\0') {
                    // The string contains non-numeric characters
                    fprintf(stderr, "Scalar value must be an integer\n\tTerminating...\n");
                    return 1;
                }
                break;
            default:
                // Invalid flag(s)
                fprintf(stderr, USAGE_ERROR, argv[0]);
                return 1;
        }
    }

    if (smatrix_size <= 0) {
        // Matrix size must be greater than 0, otherwise exit
        fprintf(stderr, "Matrix size must be greater than 0\n\tTerminating...\n");
        return 1;
    }

    // Clang-Tidy: Rand() has limited randomness
    // This is ignored since the purpose of the program is to demonstrate the operations on matrices
    // and the randomness of the matrices is not crucial for the demonstration
    const unsigned int srand_seed = time(NULL); // Seed for random number generation
    srand(srand_seed);

    // PART 1:
    // Generate two square matrices of size n x n
    printf("\n# addition of two %dx%d random matrices\n", smatrix_size, smatrix_size);

    MatrixNode *matrixA = NULL;
    generate_randomly_filled_smatrix(&matrixA, smatrix_size);
    printf("A:\n");
    print_smatrix(&matrixA, smatrix_size);

    MatrixNode *matrixB = NULL;
    generate_randomly_filled_smatrix(&matrixB, smatrix_size);
    printf("B:\n");
    print_smatrix(&matrixB, smatrix_size);

    MatrixNode *matrixC = NULL;
    add_two_smatrices(&matrixC, &matrixA, &matrixB, smatrix_size);
    printf("C:\n");
    print_smatrix(&matrixC, smatrix_size);

    free_smatrix(&matrixA);
    matrixA = NULL;
    free_smatrix(&matrixB);
    matrixB = NULL;
    free_smatrix(&matrixC);
    matrixC = NULL;

    // PART 2:
    // Scalar multiplication of a square matrix
    printf("\n# scalar multiplication of a %dx%d matrix by %d\n", smatrix_size, smatrix_size, scalar_value);
    printf("s: %d\n", scalar_value);

    generate_randomly_filled_smatrix(&matrixA, smatrix_size);
    printf("A:\n");
    print_smatrix(&matrixA, smatrix_size);

    smatrix_scalar_multiplication(&matrixB, &matrixA, smatrix_size, scalar_value);
    printf("C:\n");
    print_smatrix(&matrixB, smatrix_size);

    free_smatrix(&matrixA);
    matrixA = NULL;
    free_smatrix(&matrixB);
    matrixB = NULL;

    // PART 3:
    // Transpose of a square matrix
    printf("\n# transposition of a random matrix\n");

    generate_randomly_filled_smatrix(&matrixA, smatrix_size);
    printf("A:\n");
    print_smatrix(&matrixA, smatrix_size);

    transpose_smatrix(&matrixB, &matrixA, smatrix_size);
    printf("A.T:\n");
    print_smatrix(&matrixB, smatrix_size);

    free_smatrix(&matrixA);
    matrixA = NULL;
    free_smatrix(&matrixB);
    matrixB = NULL;

    // The program has run successfully
    return 0;
}

/**
 * @brief Creates a new node for the matrix
 * @param value The value to be stored in the node
 * @param row_index The row index of the value in the matrix
 * @param col_index The column index of the value in the matrix
 * @return A pointer to the newly created node, NULL if memory allocation fails
 */
MatrixNode *create_node(const int value, const int row_index, const int col_index) {
    MatrixNode *new_node = malloc(sizeof(MatrixNode));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n\tTerminating...\n");
        return NULL;
    }

    new_node->value = value;

    new_node->row_index = row_index;
    new_node->col_index = col_index;

    new_node->next = new_node; // Circular doubly linked list
    new_node->prev = new_node; // Temporarily set to itself

    return new_node;
}

/**
 * @brief Inserts a new node to the doubly linked list
 * @param head The head of the list
 * @param value The value to be stored in the node
 * @param row_index The row index of the value in the matrix
 * @param col_index The column index of the value in the matrix
 */
void insert_node(MatrixNode **head, const int value, const int row_index, const int col_index) {
    if (value == 0) {
        // The value is 0, no need to store it
        return;
    }

    MatrixNode *new_node = create_node(value, row_index, col_index);

    if (new_node == NULL) {
        // Memory allocation failed
        return;
    }

    if (*head == NULL) {
        // The list is empty
        *head = new_node;
        return;
    }

    // The list is not empty
    MatrixNode *last_node = (*head)->prev;

    new_node->next = *head;
    new_node->prev = last_node;

    last_node->next = new_node; // Put the new node at the end of the list
    (*head)->prev = new_node; // Update the previous pointer of the head
}

/**
 * @brief Generates a square matrix of size n x n with random values
 * @param head The head of the list
 * @param smatrix_size The size of the square matrix
 */
void generate_randomly_filled_smatrix(MatrixNode **head, const int smatrix_size) {
    for (int row = 0; row < smatrix_size; row = row + 1) {
        for (int col = 0; col < smatrix_size; col = col + 1) {
            const int random_value = rand() % 10; // Generate a random value between 0 and 9
            insert_node(head, random_value, row, col);
        }
    }
}

/**
 * @brief Retrieves the value at the specified row and column indices in the matrix
 * @param head The head of the list
 * @param row_index The row index of the value in the matrix
 * @param col_index The column index of the value in the matrix
 * @return The value at the specified row and column indices, 0 if the value is not found
 */
int get_smatrix_entry(const MatrixNode *head, const int row_index, const int col_index) {
    const MatrixNode *current = head;

    do {
        // The value is found in the linked list
        if (current->row_index == row_index && current->col_index == col_index) {
            return current->value;
        }

        // The value is not in the list, hence return 0
        if (row_index < current->row_index && col_index < current->col_index) {
            return 0;
        }

        current = current->next;
    } while (head != current);

    current = NULL; // Good practice to reset the pointer

    return 0; // The value is not found
}

/**
 * @brief Adds two square matrices of size n x n
 * @param matrixR The head of the list for the result matrix
 * @param matrixF The head of the list for the first matrix
 * @param matrixS The head of the list for the second matrix
 * @param smatrix_size The size of the square matrices
 */
void add_two_smatrices(MatrixNode **matrixR, MatrixNode **matrixF, MatrixNode **matrixS, const int smatrix_size) {
    for (int row = 0; row < smatrix_size; row = row + 1) {
        for (int col = 0; col < smatrix_size; col = col + 1) {
            const int valueA = get_smatrix_entry(*matrixF, row, col);
            const int valueB = get_smatrix_entry(*matrixS, row, col);
            const int sum = valueA + valueB;
            insert_node(matrixR, sum, row, col);
        }
    }
}

/**
 * @brief Multiplies a square matrix of size n x n by a scalar value
 * @param matrixR The head of the list for the result matrix
 * @param matrixI The head of the list for the input matrix
 * @param smatrix_size The size of the square matrix
 * @param scalar_value The scalar value
 */
void smatrix_scalar_multiplication(MatrixNode **matrixR, MatrixNode **matrixI,
                                   const int smatrix_size, const int scalar_value) {
    for (int row = 0; row < smatrix_size; row = row + 1) {
        for (int col = 0; col < smatrix_size; col = col + 1) {
            const int value = get_smatrix_entry(*matrixI, row, col);
            const int product = value * scalar_value;
            insert_node(matrixR, product, row, col);
        }
    }
}

/**
 * @brief Transposes a square matrix of size n x n
 * @param matrixR The head of the list for the result matrix
 * @param matrixI The head of the list for the input matrix
 * @param smatrix_size The size of the square matrix
 */
void transpose_smatrix(MatrixNode **matrixR, MatrixNode **matrixI, const int smatrix_size) {
    for (int row = 0; row < smatrix_size; row = row + 1) {
        for (int col = 0; col < smatrix_size; col = col + 1) {
            const int value = get_smatrix_entry(*matrixI, row, col);
            insert_node(matrixR, value, col, row);
        }
    }
}

/**
 * @brief Prints the square matrix onto the console
 * @param head The head of the list
 * @param smatrix_size The size of the square matrix
 */
void print_smatrix(MatrixNode **head, const int smatrix_size) {
    if (*head == NULL) {
        // The list is empty, nothing to print, return
        return;
    }

    for (int row = 0; row < smatrix_size; row = row + 1) {
        for (int col = 0; col < smatrix_size; col = col + 1) {
            const int value = get_smatrix_entry(*head, row, col);
            printf("  %3d ", value);
        }

        printf("\n"); // Move to the next row
    }
}

/**
 * @brief Frees the memory allocated for the doubly linked list, i.e. the square matrix
 * @param head The head of the list
 */
void free_smatrix(MatrixNode **head) {
    if (*head == NULL) {
        // The list is empty, nothing to free, return
        return;
    }

    MatrixNode *current = *head;
    MatrixNode *next = NULL;

    do {
        next = current->next;
        free(current);
        current = next;
    } while (current != *head);

    // Reset the head and pointers -- good practice
    *head = NULL;
    current = NULL;
    next = NULL;
}
