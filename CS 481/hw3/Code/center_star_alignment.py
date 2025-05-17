# Types ──────────────────────────────────────────────────────────────────────────────────────────────────────────────
# These types are used where the interpreter raises warnings about the types of some variables.
# Hence, the whole point is to silence these warnings in such cases.
# This is why only selected variables are annotated with types, and not all of them.
# As python is a dynamically typed language, the type hints are not strictly enforced, they can safely be removed.
type ScoreMatrix = list[list[int | float]]
type BacktrackMatrix = list[list[list[None | tuple]]]
type FinalAlignmentsList = list[None | int]
# ────────────────────────────────────────────────────────────────────────────────────────────────────────────────────


def needleman_wunsch_affine(sequence_q: str, sequence_s: str, match_score: int, mismatch_penalty: int, gap_opening_penalty: int, gap_extending_penalty: int) -> tuple[str, str, int]:
    """
    Needleman-Wunsch algorithm with affine gap penalties,
        i.e. global alignment of two sequences with affine gap penalties.
    :param sequence_q: The query sequence to be aligned against the subject sequence.
    :param sequence_s: The subject sequence to be aligned against the query sequence.
    :param match_score: The score for a match between two residues.
    :param mismatch_penalty: The penalty for a mismatch between two residues.
    :param gap_opening_penalty: The penalty for opening a gap in either sequence.
    :param gap_extending_penalty: The penalty for extending a gap in either sequence.
    :return: A tuple containing the aligned query sequence, aligned subject sequence, and the alignment score.
    """
    length_sequence_q = len(sequence_q)
    length_sequence_s = len(sequence_s)

    minus_infinity = float('-inf')  # negative infinity for initialising the matrices

    # dynamic‑programming matrices
    matrix_scores: ScoreMatrix = [[minus_infinity] * (length_sequence_s + 1) for _ in range(length_sequence_q + 1)]
    matrix_gaps_q: ScoreMatrix = [[minus_infinity] * (length_sequence_s + 1) for _ in range(length_sequence_q + 1)]
    matrix_gaps_s: ScoreMatrix = [[minus_infinity] * (length_sequence_s + 1) for _ in range(length_sequence_q + 1)]

    # backtrack matrix
    matrix_backtrack: BacktrackMatrix = [
        [[None] * 3 for _ in range(length_sequence_s + 1)] for _ in range(length_sequence_q + 1)
    ]

    # initial conditions
    matrix_scores[0][0] = 0
    for i in range(1, length_sequence_q + 1):
        matrix_gaps_q[i][0] = gap_opening_penalty + gap_extending_penalty * i
        matrix_backtrack[i][0][1] = (i - 1, 0, 1)

    for j in range(1, length_sequence_s + 1):
        matrix_gaps_s[0][j] = gap_opening_penalty + gap_extending_penalty * j
        matrix_backtrack[0][j][2] = (0, j - 1, 2)

    for i in range(1, length_sequence_q + 1):
        for j in range(1, length_sequence_s + 1):
            mis_match = match_score if sequence_q[i - 1] == sequence_s[j - 1] else mismatch_penalty

            # matrix_scores – align the two residues
            best_score, source_state = max(
                (matrix_scores[i - 1][j - 1], 0),
                (matrix_gaps_s[i - 1][j - 1], 2),
                (matrix_gaps_q[i - 1][j - 1], 1),
            )
            matrix_scores[i][j] = best_score + mis_match
            matrix_backtrack[i][j][0] = (i - 1, j - 1, source_state)

            # matrix_gaps_q – extend/open a gap in subject sequence
            best_score, source_state = max(
                (matrix_scores[i - 1][j] + gap_opening_penalty + gap_extending_penalty, 0),
                (matrix_gaps_s[i - 1][j] + gap_opening_penalty + gap_extending_penalty, 2),
                (matrix_gaps_q[i - 1][j] + gap_extending_penalty, 1),
            )
            matrix_gaps_q[i][j] = best_score
            matrix_backtrack[i][j][1] = (i - 1, j, source_state)

            # matrix_gaps_s – extend/open a gap in query sequence
            best_score, source_state = max(
                (matrix_scores[i][j - 1] + gap_opening_penalty + gap_extending_penalty, 0),
                (matrix_gaps_q[i][j - 1] + gap_opening_penalty + gap_extending_penalty, 1),
                (matrix_gaps_s[i][j - 1] + gap_extending_penalty, 2),
            )
            matrix_gaps_s[i][j] = best_score
            matrix_backtrack[i][j][2] = (i, j - 1, source_state)

    score, state = max((matrix_scores[length_sequence_q][length_sequence_s], 0),
                       (matrix_gaps_s[length_sequence_q][length_sequence_s], 2),
                       (matrix_gaps_q[length_sequence_q][length_sequence_s], 1))

    # traceback
    aligned_sequence_q, aligned_sequence_s = [], []
    i, j = length_sequence_q, length_sequence_s
    while i > 0 or j > 0:
        if state == 0:
            aligned_sequence_q.append(sequence_q[i - 1])
            aligned_sequence_s.append(sequence_s[j - 1])
        elif state == 1:
            aligned_sequence_q.append(sequence_q[i - 1])
            aligned_sequence_s.append('-')
        else:
            aligned_sequence_q.append('-')
            aligned_sequence_s.append(sequence_s[j - 1])

        i, j, state = matrix_backtrack[i][j][state]

    return ''.join(reversed(aligned_sequence_q)), ''.join(reversed(aligned_sequence_s)), score


def merge_alignments(multiple_alignments: list[chr], current_center_index: int, gapped_center: str, gapped_sequence: str) -> None:
    """
    Merge the new alignment into the existing multiple alignment.
    :param multiple_alignments: The existing multiple alignment at a given state.
    :param current_center_index: The index of the center sequence in the multiple alignment.
    :param gapped_center: The gapped center sequence, aligned with the new sequence.
    :param gapped_sequence: The new sequence to be aligned with the center sequence.
    :return: None
    """
    current_center_row = multiple_alignments[current_center_index]

    i, j = 0, 0  # indices for current_center_row and gapped_center
    new_center = []
    while i < len(current_center_row) or j < len(gapped_center):
        original_char = current_center_row[i] if i < len(current_center_row) else None
        new_center_char = gapped_center[j] if j < len(gapped_center) else None

        # centre finished but new_centre still has columns → append
        if original_char is None:
            for k, row in enumerate(multiple_alignments):
                if k != current_center_index:  # other rows get a gap
                    row.append('-')
            current_center_row.append(new_center_char)  # centre gets its character
            new_center.append(gapped_sequence[j])
            i += 1
            j += 1
            continue

        # need an extra gap column (new_center_char == '-', original_char != '-')
        if new_center_char == '-' and original_char != '-':
            for row in multiple_alignments:  # insert gap everywhere
                row.insert(i, '-')
            new_center.append(gapped_sequence[j])
            i += 1
            j += 1
            continue

        # centre already has a gap but new_centre now carries a residue
        if original_char == '-' and new_center_char != '-':
            new_center.append('-')
            i += 1
            continue

        # normal column (both are residues, or both gaps)
        new_center.append(gapped_sequence[j] if j < len(gapped_sequence) else '-')
        i += 1
        j += 1

    multiple_alignments.append(new_center)


def center_star_alignment(sequences: list[str], match_score: int, mismatch_penalty: int, gap_opening_penalty: int, gap_extending_penalty: int) -> list[str]:
    """
    Center star alignment algorithm,
        i.e. progressive alignment of multiple sequences using the center star method.
    :param sequences: The list of sequences to be aligned with each other.
    :param match_score: The score for a match between two residues.
    :param mismatch_penalty: The penalty for a mismatch between two residues.
    :param gap_opening_penalty: The penalty for opening a gap in either sequence.
    :param gap_extending_penalty: The penalty for extending a gap in either sequence.
    :return: A list of aligned sequences.
    """
    number_of_sequences = len(sequences)

    matrix_distances = [[0] * number_of_sequences for _ in range(number_of_sequences)]
    for i in range(number_of_sequences):
        for j in range(i + 1, number_of_sequences):
            _, _, alignment_score = needleman_wunsch_affine(
                sequences[i], sequences[j],
                match_score, mismatch_penalty, gap_opening_penalty, gap_extending_penalty,
            )
            matrix_distances[i][j] = matrix_distances[j][i] = -alignment_score

    # 2. choose the center_sequence_index (smallest Σ matrix_distances)
    total_distances_per_sequence = [sum(row) for row in matrix_distances]
    center_sequence_index = min(range(number_of_sequences), key=total_distances_per_sequence.__getitem__)
    center_sequence = sequences[center_sequence_index]

    # 3. progressive build
    multiple_alignments = [list(center_sequence)]
    row_of_sequence: FinalAlignmentsList = [None] * number_of_sequences
    row_of_sequence[center_sequence_index] = 0

    for i, sequence in enumerate(sequences):
        if i != center_sequence_index:
            gapped_center, gapped_sequence, _ = needleman_wunsch_affine(
                center_sequence, sequence,
                match_score, mismatch_penalty, gap_opening_penalty, gap_extending_penalty,
            )
            merge_alignments(multiple_alignments, 0, gapped_center, gapped_sequence)
            row_of_sequence[i] = len(multiple_alignments) - 1

    # 4. strip columns
    columns = list(zip(*multiple_alignments))
    columns_to_keep = [col for col in columns if any(ch != '-' for ch in col)]

    # 5. rebuild strings in the original order
    return [''.join(col[row_of_sequence[i]] for col in columns_to_keep) for i in range(number_of_sequences)]


if __name__ == '__main__':
    def main() -> None:
        """
        The test function for the center star alignment algorithm.
        It uses three example sequences and prints the aligned sequences.
        :return: None
        """
        example_sequences = [
            'MGIKGLTGLLSENAPKCMKDHEMKTLFGRKVAIDASMSIYQFLIAVRQQE',
            'MGIKGLTQVIGDTAPTAIKENEIKNYFGRKVAIDASMSIYQFLIAVRSE',
            'MGIKGLTQTRGDTAPTAIKEIKNYFGRKVVIDASMSIYQFLIAVRSGET',
        ]

        match_score, mismatch_penalty, gap_opening_penalty, gap_extending_penalty = 1, -1, -1, -1

        aligned_sequences = center_star_alignment(
            example_sequences,
            match_score, mismatch_penalty, gap_opening_penalty, gap_extending_penalty
        )

        print(aligned_sequences)


    main()
