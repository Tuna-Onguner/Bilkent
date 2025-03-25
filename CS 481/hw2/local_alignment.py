def smith_waterman(pattern: str, text: str, match_score: int, mismatch_penalty: int, gap_penalty: int) -> tuple[str, str, int]:
    """
    Performs Smith-Waterman local alignment between a pattern and a text
    :param pattern: The first pattern to align
    :param text: The second pattern to align
    :param match_score: The score for a match
    :param mismatch_penalty: The penalty for a mismatch
    :param gap_penalty: The penalty for a gap
    :return: A tuple containing the aligned pattern, aligned text, and the alignment score respectively
    """
    """
    REFERENCES:
    - The original paper that the algorithm is described in:
        Smith, T. F., & Waterman, M. S. (1981). Identification of common molecular subpatterns. 
            Journal of Molecular Biology, 147(1), 195–197. https://doi.org/10.1016/0022-2836(81)90087-5 

    - GitHub repository applied for the implementation:
        Slavina, A. (2020). Smith-Waterman-Algorithm [Computer software]. GitHub.
            https://github.com/slavianap/Smith-Waterman-Algorithm

    - For CIGAR and MD:Z strings, following resources were used in addition to the ones given in the assignment document:
        CIGAR string. USEARCH. (n.d.-a). https://www.drive5.com/usearch/manual/cigar.html
        ChatGPT. (2025, March 9). Explanation of the MD:Z string representation in DNA and protein alignments. 
            OpenAI. Retrieved from https://chatgpt.com/. Original Conversation Link: https://chatgpt.com/share/67cd890d-5f04-8002-873d-64a3d8d7a1a4
    """
    # Initialize scoring matrix and traceback matrix
    rows, cols = len(pattern) + 1, len(text) + 1
    score_matrix = [[0] * cols for _ in range(rows)]
    traceback = [[None] * cols for _ in range(rows)]

    max_score, max_pos = 0, (0, 0)  # Track the maximum score and its position, initialize to 0

    # Fill matrices
    for i in range(1, rows):
        for j in range(1, cols):
            # Calculate match/mismatch score
            match = match_score if pattern[i - 1] == text[j - 1] else mismatch_penalty
            # Calculate scores for three possible directions
            diag = score_matrix[i - 1][j - 1] + match  # Diagonal, i.e. match or mismatch
            up = score_matrix[i - 1][j] + gap_penalty  # Up, i.e. insertion
            left = score_matrix[i][j - 1] + gap_penalty  # Left, i.e. deletion
            score_matrix[i][j] = max(0, diag, up, left)  # Fill the scoring matrix

            # Track max score position
            if score_matrix[i][j] > max_score:
                max_score = score_matrix[i][j]
                max_pos = (i, j)  # Update max score position

            # Determine traceback direction
            if score_matrix[i][j] == diag:
                traceback[i][j] = 'D'  # Diagonal
            elif score_matrix[i][j] == up:
                traceback[i][j] = 'U'  # Up
            elif score_matrix[i][j] == left:
                traceback[i][j] = 'L'  # Left
            else:
                traceback[i][j] = None  # End of alignment

    # Traceback
    aligned_pattern = ""
    aligned_text = ""

    i, j = max_pos
    while traceback[i][j] is not None:

        # Case 1: Diagonal (Match/Mismatch)
        if traceback[i][j] == 'D':  # D stands for diagonal, i.e. match or mismatch
            aligned_pattern = pattern[i - 1] + aligned_pattern  # Place the pattern character in the aligned pattern
            aligned_text = text[j - 1] + aligned_text  # Place the text character in the aligned text
            # Move diagonally
            i -= 1
            j -= 1

        # Case 2: Up (Insertion)
        elif traceback[i][j] == 'U':  # U stands for up, i.e. insertion
            aligned_pattern = pattern[i - 1] + aligned_pattern  # Place the pattern character in the aligned pattern
            aligned_text = '-' + aligned_text  # Insert a gap in the text
            i -= 1  # Move up

        # Case 3: Left (Deletion)
        elif traceback[i][j] == 'L':  # L stands for left, i.e. deletion
            aligned_pattern = '-' + aligned_pattern  # Insert a gap in the pattern
            aligned_text = text[j - 1] + aligned_text  # Place the text character in the aligned text
            j -= 1  # Move to left

    return aligned_pattern, aligned_text, max_score
