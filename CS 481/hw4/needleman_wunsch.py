"""
This is the Type-2 code clone of the needleman_wunsch algorithm I had used in the Homework #2.
Only minor changes have been applied, like changing variable names etc.

'You can use your code from HW2.' from the assignment PDF, page 2, UPGMA, Line 3.
"""


def needleman_wunsch(query_sequence: str, subject_sequence: str,
                     match_score: int, mismatch_penalty: int, gap_penalty: int) -> tuple[str, str, int]:
    """
    Performs Needleman-Wunsch, i.e. global alignment, between two sequences.
    :param query_sequence: The query sequence to be aligned against the subject sequence
    :param subject_sequence: The subject sequence to be aligned against the query sequence.
    :param match_score: The score for a match between two residues.
    :param mismatch_penalty: The penalty for a mismatch between two residues.
    :param gap_penalty: The penalty for opening a gap in either sequence.
    :return: A tuple containing the aligned query, aligned subject, and the alignment score respectively
    """
    """
    REFERENCES:
    - The original paper that the algorithm is described in:
        Needleman, S. B., & Wunsch, C. D. (1970). A general method applicable to the search for similarities in the 
            amino acid pattern of two proteins. Journal of Molecular Biology, 48(3), 443–453. 
            https://doi.org/10.1016/0022-2836(70)90057-4

    - GitHub repository applied for the implementation:
        ATalhaTimur. (2024). Needleman-Wunsch-Algorithm [Computer software]. GitHub. 
            https://github.com/ATalhaTimur/Needleman-Wunsch-Algorithm
    """
    # Initialize scoring matrix
    n_rows, n_cols = len(query_sequence) + 1, len(subject_sequence) + 1
    matrix = [[0] * n_cols for _ in range(n_rows)]  # Fill the matrix by zeros initially
    # matrix stands for 2D list

    # Initialize first row and column with gap penalties
    for i in range(n_rows):
        # Fill the first column with gap penalties
        matrix[i][0] = i * gap_penalty

    # Fill the first row with gap penalties
    for j in range(n_cols):
        matrix[0][j] = j * gap_penalty

    # Fill the matrix
    for i in range(1, n_rows):
        for j in range(1, n_cols):
            is_match = query_sequence[i - 1] == subject_sequence[j - 1]
            mis_match = matrix[i - 1][j - 1] + (match_score if is_match else mismatch_penalty)
            deletion = matrix[i - 1][j] + gap_penalty
            insertion = matrix[i][j - 1] + gap_penalty
            # Find the maximum resulting state and put it in the dp matrix
            matrix[i][j] = max(mis_match, deletion, insertion)

    # Initialize resulting sequences as empty
    aligned_query_sequence = ""
    aligned_subject_sequence = ""

    # Traceback to get the optimal alignment
    i, j = len(query_sequence), len(subject_sequence)
    while i > 0 or j > 0:  # While we haven't reached the top-left corner, keep tracing back
        current_score = matrix[i][j]  # Current score
        is_match = query_sequence[i - 1] == subject_sequence[j - 1]  # Check if the current characters are a match

        # Case 1: Deletion
        if current_score == matrix[i - 1][j] + gap_penalty:  # If the current score is from a deletion
            aligned_query_sequence = query_sequence[i - 1] + aligned_query_sequence  # Add the current character to the aligned query
            aligned_subject_sequence = "-" + aligned_subject_sequence  # Add a gap to the aligned subject
            i -= 1  # Move up

        # Case 2: Insertion
        elif current_score == matrix[i][j - 1] + gap_penalty:
            aligned_query_sequence = "-" + aligned_query_sequence  # Add a gap to the aligned query
            aligned_subject_sequence = subject_sequence[j - 1] + aligned_subject_sequence  # Add the current character to the aligned subject
            j -= 1  # Move to left

        # Case 3: Match or Mismatch
        elif current_score == matrix[i - 1][j - 1] + (match_score if is_match else mismatch_penalty):
            aligned_query_sequence = query_sequence[i - 1] + aligned_query_sequence  # Add the current character to the aligned query
            aligned_subject_sequence = subject_sequence[j - 1] + aligned_subject_sequence  # Add the current character to the aligned subject
            # Move diagonally
            i -= 1
            j -= 1

    return aligned_query_sequence, aligned_subject_sequence, matrix[-1][-1]  # matrix[-1][-1] -> last row and last column, i.e. the final score of the alignment
