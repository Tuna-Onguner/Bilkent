def needleman_wunsch(pattern: str, text: str, match_score: int, mismatch_penalty: int, gap_penalty: int) -> tuple[str, str, int]:
    """
    Performs Needleman-Wunsch global alignment between a pattern and a text
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
        Needleman, S. B., & Wunsch, C. D. (1970). A general method applicable to the search for similarities in the 
            amino acid pattern of two proteins. Journal of Molecular Biology, 48(3), 443–453. 
            https://doi.org/10.1016/0022-2836(70)90057-4

    - GitHub repository applied for the implementation:
        ATalhaTimur. (2024). Needleman-Wunsch-Algorithm [Computer software]. GitHub. 
            https://github.com/ATalhaTimur/Needleman-Wunsch-Algorithm

    - For CIGAR and MD:Z strings, following resources were used in addition to the ones given in the assignment document:
        CIGAR string. USEARCH. (n.d.-a). https://www.drive5.com/usearch/manual/cigar.html
        ChatGPT. (2025, March 9). Explanation of the MD:Z string representation in DNA and protein alignments. 
            OpenAI. Retrieved from https://chatgpt.com/. Original Conversation Link: https://chatgpt.com/share/67cd890d-5f04-8002-873d-64a3d8d7a1a4
    """
    # Initialize scoring matrix
    rows, cols = len(pattern) + 1, len(text) + 1
    matrix = [[0] * cols for _ in range(rows)]

    # Initialize first row and column with gap penalties
    for i in range(rows):
        matrix[i][0] = i * gap_penalty  # Fill the first column with gap penalties
    for j in range(cols):
        matrix[0][j] = j * gap_penalty  # Fill the first row with gap penalties

    # Fill the matrix
    for i in range(1, rows):
        for j in range(1, cols):
            match = matrix[i - 1][j - 1] + (match_score if pattern[i - 1] == text[j - 1] else mismatch_penalty)
            delete = matrix[i - 1][j] + gap_penalty
            insert = matrix[i][j - 1] + gap_penalty
            matrix[i][j] = max(match, delete, insert)

    # Traceback to get the optimal alignment
    aligned_pattern = ""
    aligned_text = ""

    i, j = len(pattern), len(text)
    while i > 0 or j > 0:  # While we haven't reached the top-left corner, keep tracing back
        current_score = matrix[i][j]  # Current score
        is_match = pattern[i - 1] == text[j - 1]  # Check if the current characters are a match

        # Case 1: Deletion
        if current_score == matrix[i - 1][j] + gap_penalty:  # If the current score is from a deletion
            aligned_pattern = pattern[i - 1] + aligned_pattern  # Add the current character to the aligned pattern
            aligned_text = "-" + aligned_text  # Add a gap to the aligned text
            i -= 1  # Move up

        # Case 2: Insertion
        elif current_score == matrix[i][j - 1] + gap_penalty:
            aligned_pattern = "-" + aligned_pattern  # Add a gap to the aligned pattern
            aligned_text = text[j - 1] + aligned_text  # Add the current character to the aligned text
            j -= 1  # Move to left

        # Case 3: Match or Mismatch
        elif current_score == matrix[i - 1][j - 1] + (match_score if is_match else mismatch_penalty):
            aligned_pattern = pattern[i - 1] + aligned_pattern  # Add the current character to the aligned pattern
            aligned_text = text[j - 1] + aligned_text  # Add the current character to the aligned text
            # Move diagonally
            i -= 1
            j -= 1

    return aligned_pattern, aligned_text, matrix[-1][-1]
