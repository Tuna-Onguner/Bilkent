import os
import argparse
from typing import Any
from center_star_alignment import center_star_alignment
from phylip import Phylip


def parse_command_line_arguments() -> tuple[Any, Any, int, int, int, int]:
    """
    Parses command line arguments:
    -i: Path to the input file containing sequences
    -o: Path to the output file to save results
    -s: Scores in the format MATCH:MISMATCH:OPENING:GAP
    :return: A tuple containing the input file path, output file path, and scores for match, mismatch, opening, and gap
    :raises argparse.ArgumentTypeError if the input file does not exist or if the scores are not in the correct format
    """

    def validate_file(filepath: str) -> str:
        """
        Validates if the given file path exists, if not raises an argument error
        :param filepath: Path to the file to be validated
        :return: The file path if it exists
        :raises argparse.ArgumentTypeError if the file does not exist
        """
        if not os.path.isfile(filepath):
            raise argparse.ArgumentTypeError(f"File '{filepath}' does not exist.")
        return filepath

    parser = argparse.ArgumentParser(
        description="Bilkent University Spring 2025 CS481 - Bioinformatics Algorithms Homework Assignment #3"
        # This description will be displayed when the user runs the program with the `-h` flag
        # -h flag is provided by default by the argparse module
    )

    # Add command line arguments
    parser.add_argument("-i", type=validate_file, required=True, dest="input_filepath", help="Path to the input file containing sequences")
    parser.add_argument("-o", type=str, required=True, dest="output_filepath", help="Path to the output file to save results")
    parser.add_argument("-s", type=str, required=True, dest="scores", help="Scores in the format MATCH:MISMATCH:OPENING:GAP")

    args = parser.parse_args()  # Parse the arguments

    try:
        # Split the scores into their respective components
        scores = [int(score) for score in args.scores.strip().split(":")]
        if len(scores) != 4:
            raise ValueError("Scores must be in the format MATCH:MISMATCH:OPENING:GAP")
        # Unpack the scores into their respective variables
        match, mismatch, opening, gap = scores
    except ValueError as value_error:  # Catch any ValueError that occurs during the conversion of scores
        parser.error(f"Invalid scores format: {value_error}")

    return args.input_filepath, args.output_filepath, match, mismatch, opening, gap


def read_sequences_from_fasta_and_lexicize(filepath: str) -> dict[str, str]:
    """
    Reads sequences from a FASTA file and store them in a dictionary
    :param filepath: Path to the FASTA file
    :return: Dictionary containing the sequence names as keys and the sequences as values
    """
    sequences = {}

    with open(filepath, "r") as fasta:
        sequence_name = ""
        sequence = ""
        # Read the file line by line
        for line in fasta:
            if line.startswith(">"):  # If the line starts with ">", it is a new sequence
                if sequence_name:  # Add the previous sequence to the dictionary
                    sequences[sequence_name] = sequence

                sequence_name = line.strip()[1:]  # Remove the ">" and the newline character
                sequence = ""  # Reset the sequence
            # If the line does not start with ">", it is part of the sequence
            else:
                sequence += line.strip()  # Add the line to the sequence

        if sequence_name:  # Add the last sequence to the dictionary
            sequences[sequence_name] = sequence

    return sequences


def main() -> None:
    """
    Main function to execute the program.
    It reads sequences from a FASTA file, performs center star alignment,
        and writes the aligned sequences to a Phylip file.
    The affine penalties are provided as command line arguments.
    :raise ValueError: If no sequences are found in the input file
    """
    input_filepath, output_filepath, match, mismatch, opening, gap = parse_command_line_arguments()
    sequences = read_sequences_from_fasta_and_lexicize(input_filepath)

    if not sequences:  # Check if the dictionary is empty, meaning no sequences were found, nothing to align
        raise ValueError("No sequences found in the input file.")

    # Perform center star alignment
    aligned_sequences = center_star_alignment(
        list(sequences.values()), match, mismatch, opening, gap
    )

    # Create a Phylip object and write the aligned sequences to the output file
    # The length of the longest sequence name is used to determine the width of the first column in the output file
    phylip = Phylip()
    phylip.write_phylip(output_filepath, dict(zip(sequences.keys(), aligned_sequences)))


if __name__ == '__main__':
    main()
