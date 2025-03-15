#
# Bilkent University
# Semester of Spring 2025
#
# Department of Computer Engineering
# CS481 Bioinformatics Algorithms
#
# Homework Assignment #2: Pattern Alignment
#
# Author: Deniz Tuna ONGUNER
# Bilkent ID: 22001788
#
# Section: 1
# Instructor: Can ALKAN
#
# Date: 13.03.2025 (DD.MM.YYYY)
#
# main.py:
# Main file for Homework Assignment #2
#
# Interpreter: Python 3.10.2 or newer
# Encoding: UTF-8
#

import re  # Import the re module for regular expressions
import os  # Import the os module to check if the files exist
import argparse  # Import the argparse module to parse command line arguments
from enum import Enum  # Import the Enum class for alignment types
from global_alignment import needleman_wunsch
from local_alignment import smith_waterman
import multiprocessing as mp


class AlignmentMode(Enum):
    """
    Enum class for alignment types/modes
    - LOCAL: Local alignment for patterns
    - GLOBAL: Global alignment for patterns
    """
    LOCAL = "local"
    GLOBAL = "global"


class AlignmentResult:
    """
    Class to store the result of an alignment operation
    """

    def __init__(self, pattern: str, text: str, score: int, cigar: str, mdz: str) -> None:
        """
        Constructor with overlap for global alignment results
        :param pattern: The first pattern, i.e. reference pattern
        :param text: The second pattern, i.e. text
        :param score: The score of the alignment process
        :param cigar: The CIGAR string of the alignment
        :param mdz: The MD:Z string of the alignment
        """
        self.pattern = pattern
        self.text = text
        self.score = score
        self.cigar = cigar
        self.mdz = mdz

    def __str__(self) -> str:
        """
        String representation of an alignment result
        :return: The string representation of an alignment result including the pattern, text, score, CIGAR and MD:Z
        """
        return f"pattern={self.pattern}\ntext={self.text}\nScore={self.score}\nCIGAR={self.cigar}\nMD:Z={self.mdz}"


class PenaltyScores:
    """
    Class to store penalties for match, mismatch, and gap
    """

    def __init__(self, match: int, mismatch: int, gap: int) -> None:
        """
        Constructor for PenaltyScores class
        :param match: The match score
        :param mismatch: The mismatch penalty
        :param gap: The gap penalty
        """
        self.match = match
        self.mismatch = mismatch
        self.gap = gap


def validate_file(filepath: str) -> str:
    """
    Validates if the given file path exists, if not raise an argument error
    :param filepath: Path to the file to be validated
    :return: The file path if it exists
    :raises argparse.ArgumentTypeError if the file does not exist
    """
    if not os.path.isfile(filepath):
        raise argparse.ArgumentTypeError(f"File '{filepath}' does not exist.")
    return filepath


def parse_command_line_arguments() -> argparse.Namespace:
    """
    Parses command line arguments
    :return: argparse.Namespace type object containing the parsed arguments
    """
    parser = argparse.ArgumentParser(description="Bilkent University Spring 2025 CS481 - Bioinformatics Algorithms Homework Assignment #2")
    # This description will be displayed when the user runs the program with the `-h` flag
    # -h flag is provided by default by the argparse module

    # Mutually exclusive arguments for alignment type, either local `-l` or global `-g`
    # This is to guarantee that the user will provide only one of the alignment types and will raise an error if not
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-l", action="store_const", const=AlignmentMode.LOCAL, dest="alignment_mode", help="Perform local alignment")
    group.add_argument("-g", action="store_const", const=AlignmentMode.GLOBAL, dest="alignment_mode", help="Perform global alignment")

    # Other arguments: patterns file, texts file, output file, penalties
    parser.add_argument("-p", type=validate_file, required=True, dest="patterns_filepath", help="Path to the patterns file")
    parser.add_argument("-t", type=validate_file, required=True, dest="texts_filepath", help="Path to the texts file")
    # The validate_file function is used to validate the file paths, if the file does not exist, an error will be raised
    parser.add_argument("-o", type=str, required=True, dest="output_filepath", help="Path to the output file")
    parser.add_argument("-s", type=int, nargs=3, metavar=("MATCH", "MISMATCH", "GAP"), required=True, dest="penalties_list", help="Penalties for match, mismatch, and gap respectively")

    args = parser.parse_args()  # Parse the arguments
    # Store the penalties as a PenaltyScores object for easier access and readability
    args.penalties = PenaltyScores(*args.penalties_list)
    del args.penalties_list  # Delete the penalties list as it is no longer needed

    return args


def read_patterns_from_fasta_and_lexicize(filepath: str) -> dict:
    """
    Reads patterns from a FASTA file and store them in a dictionary
    :param filepath: Path to the FASTA file
    :return: Dictionary containing the pattern names as keys and the patterns as values
    """
    patterns = {}

    with open(filepath, "r") as fasta:
        pattern_name = ""
        pattern = ""
        # Read the file line by line
        for line in fasta:
            if line.startswith(">"):  # If the line starts with ">", it is a new pattern
                if pattern_name:  # Add the previous pattern to the dictionary
                    patterns[pattern_name] = pattern

                pattern_name = line.strip()[1:]  # Remove the ">" and the newline character
                pattern = ""  # Reset the pattern
            # If the line does not start with ">", it is part of the pattern
            else:
                pattern += line.strip()  # Add the line to the pattern

        if pattern_name:  # Add the last pattern to the dictionary
            patterns[pattern_name] = pattern

    return patterns


def write_results_to_file(results: list[AlignmentResult], output_filepath: str) -> None:
    """
    Writes the results to a file
    :param results: List of AlignmentResult objects
    :param output_filepath: Path to the output file
    :raises IOError if an error occurs while writing to the file
    """
    try:
        with open(output_filepath, "w") as output_file:
            for result in results:
                output_file.write(str(result) + "\n")
    except IOError as e:
        print(f"Error writing to the output file: {e}")
        raise e


def generate_cigar(aligned_pattern: str, aligned_text: str) -> str:
    """
    Generates the CIGAR string from the aligned pattern and text
    :param aligned_pattern: The aligned pattern
    :param aligned_text: The aligned text
    :return: The CIGAR string
    """
    cigar = []
    length = 0
    operation = None  # Operation type (M, I, D)

    for pattern_char_at_index, text_char_at_index in zip(aligned_pattern, aligned_text):
        # Match/Mismatch (in CIGAR, M represents both)
        if pattern_char_at_index == text_char_at_index:
            op = "M"
        # Deletion in pattern
        elif pattern_char_at_index == "-":
            op = "D"
        # Insertion in text
        elif text_char_at_index == "-":
            op = "I"
        else:  # Mismatch
            continue

        if operation is None:  # First operation in the alignment
            operation = op
            length = 1
        elif op == operation:  # Same operation occurs consecutively
            length += 1
        else:  # Different operation occurs
            cigar.append(f"{length}{operation}")  # Add the previous operation to the CIGAR
            operation = op  # Update the operation
            length = 1  # Reset the length

    if length > 0:  # Add the last operation to the CIGAR
        cigar.append(f"{length}{operation}")

    return "".join(cigar)


def generate_md_z(aligned_pattern: str, aligned_text: str) -> str:
    """
    Generates the MD:Z string from the aligned pattern and text
    :param aligned_pattern: The aligned pattern
    :param aligned_text: The aligned text
    :return: The MD:Z string
    """
    mdz = []
    match_count = 0
    deletion_block = ""  # Block of deletions in the aligned pattern

    for pattern_char_at_index, text_char_at_index in zip(aligned_pattern, aligned_text):
        # Match
        if pattern_char_at_index == text_char_at_index:  # Match in the pattern
            match_count += 1

            if deletion_block:  # If there is a deletion block, add it to the MD:Z string
                mdz.append(f"^{deletion_block}")
                deletion_block = ""  # Reset the deletion block
        # Deletion, insertion, or mismatch in the pattern
        else:
            if text_char_at_index == "-":  # Deletion in text
                deletion_block += pattern_char_at_index
                if match_count > 0:
                    mdz.append(str(match_count))
                    match_count = 0
            elif pattern_char_at_index == "-":  # Insertion in query (ignored in MD:Z format)
                pass  # Do nothing as insertions are ignored in the MD:Z format
            else:  # Mismatch
                if deletion_block:
                    mdz.append(f"^{deletion_block}")
                    deletion_block = ""

    if match_count > 0:  # Add the last match block to the MD:Z string
        mdz.append(str(match_count))

    if deletion_block:  # Add the last deletion block to the MD:Z string
        mdz.append(f"^{deletion_block}")

    return "".join(mdz)


def process_alignment(alignment_mode: AlignmentMode, pattern: str, text: str, match: int, mismatch: int, gap: int) -> AlignmentResult:
    """
    Processes the alignment based on the alignment mode
    :param alignment_mode: The alignment mode, either global or local
    :param pattern: The pattern to be aligned
    :param text: The text to be aligned
    :param match: The match score
    :param mismatch: The mismatch penalty
    :param gap: The gap penalty
    :return: The alignment result
    """
    match alignment_mode:  # Switch case for alignment mode
        # Global alignment
        case AlignmentMode.GLOBAL:
            aligned_pattern, aligned_text, score = needleman_wunsch(pattern, text, match, mismatch, gap)
            cigar = generate_cigar(aligned_text, aligned_pattern)
            mdz = generate_md_z(aligned_pattern, aligned_text)

        # Local alignment
        case AlignmentMode.LOCAL:
            aligned_pattern, aligned_text, score = smith_waterman(pattern, text, match, mismatch, gap)
            cigar = generate_cigar(aligned_pattern, aligned_text)
            mdz = generate_md_z(aligned_text, aligned_pattern)

        # Default case
        # This should never happen due to the required flag in the argument parser and the enum class
        # But added anyway for the sake of completeness and to avoid any potential future bugs
        case _:
            raise ValueError("Invalid alignment type")

    result = AlignmentResult(pattern, text, score, cigar, mdz)
    return result


def main() -> None:
    """
    Main function:
    - Parses the command line arguments
    - Reads the patterns and texts from the FASTA files and lexicizes them into dictionaries
    - Aligns the patterns and texts using the specified alignment mode, i.e. global or local
    - Using multiprocessing, processes the alignment in parallel for each pair of patterns and texts
    - Finds the pair with the biggest overlap for global alignment or the pair with the highest score for local alignment
    - Prints the finding
    - Writes the results to the output file with the given path and name
    :raises ValueError if the number of patterns and texts do not match
    :raises ValueError if the alignment type is invalid, although this should never happen
    """
    # Parse the command line arguments
    args = parse_command_line_arguments()

    # Read the patterns and texts from the FASTA files and lexicize them
    patterns = read_patterns_from_fasta_and_lexicize(filepath=args.patterns_filepath)
    texts = read_patterns_from_fasta_and_lexicize(filepath=args.texts_filepath)

    # Although stated that the files have an equal number of lines and the length of the strings are the same,
    # still decided to extend this line below for debugging purposes and to avoid any potential future bugs
    if len(patterns) != len(texts):  # Check if the number of patterns and texts match, they must match
        raise ValueError("Number of patterns and texts do not match")

    # Unpack the penalties for easier access
    match, mismatch, gap = args.penalties.match, args.penalties.mismatch, args.penalties.gap

    # Zip the dictionaries to iterate over pairs of patterns and texts together
    pairs = zip(patterns.values(), texts.values())

    with mp.Pool(mp.cpu_count()) as process_pool:  # Create a pool of processes with the number of CPUs
        results = process_pool.starmap(process_alignment,  # Process the alignment in parallel using multiprocessing
                                       [(args.alignment_mode, pattern, text, match, mismatch, gap)
                                        for pattern, text in pairs])

    # Flow control based on the alignment mode
    match args.alignment_mode:  # Switch case for alignment mode
        # Global alignment
        case AlignmentMode.GLOBAL:
            # Find the pair with the biggest overlap and print it
            # An overlap is defined as the longest number of consecutive matches in the alignment
            def max_overlap(c: str) -> int:
                """
                Finds the maximum overlap from the CIGAR string
                :param c: The CIGAR string
                :return: The maximum overlap from the CIGAR string given, 0 if no matches
                """
                matches = re.findall(r'(\d+)M', c)  # Find all the matches in the CIGAR string
                # The syntax (\d+)M means find all the digits followed by an M
                return max(map(int, matches)) if matches else 0

            max_overlap_pair = max(results, key=lambda p: max_overlap(p.cigar))  # Find the pair with the biggest overlap
            print("Longest overlap:\n" + str(max_overlap_pair))  # Print the pair with the biggest overlap

        # Local alignment
        case AlignmentMode.LOCAL:
            # Find the pair with the highest score and print it
            max_score_pair = max(results, key=lambda p: p.score)  # Find the pair with the highest score
            print("Highest local alignment score:\n" + str(max_score_pair))  # Print the pair with the highest score

        # Default case
        case _:
            raise ValueError("Invalid alignment type")

    # Write the results to the output file
    write_results_to_file(results, args.output_filepath)


if __name__ == '__main__':  # If the script is run directly
    main()  # Run the main function
