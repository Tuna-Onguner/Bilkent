import os
import argparse
from UPGMA import UPGMA


def parse_command_line_arguments() -> tuple[str, str, int, int, int]:
    """
    Parses command line arguments:
    -i: Path to the input file containing sequences.
    -t: Path to the output file to save results.
    -s: Scores in the format `MATCH MISMATCH GAP`, separated by single spaces.
    :return: A tuple containing the input file path, output file path, and scores for match, mismatch, and gap.
    :raises: argparse.ArgumentTypeError if the input file does not exist or if the scores are not in the correct format
    """

    def validate_file(filepath: str) -> str:
        """
        Validates if the given file path exists, if not raises an argument error.
        :param filepath: Path to the file to be validated.
        :return: The file path if it exists.
        :raises: argparse.ArgumentTypeError if the file does not exist.
        """
        if not os.path.isfile(filepath):
            raise argparse.ArgumentTypeError(f"File '{filepath}' does not exist.")
        return filepath

    parser = argparse.ArgumentParser(
        description="Bilkent University Spring 2025 CS481 - Bioinformatics Algorithms Homework Assignment #4"
        # This description will be displayed when the user runs the program with the `-h` flag
        # -h flag is provided by default by the argparse module
    )

    # Add command line arguments
    parser.add_argument("-i", type=validate_file, required=True, dest="input_filepath", help="Path to the input file containing sequences")
    parser.add_argument("-t", type=str, required=True, dest="output_filepath", help="Path to the output file to save results")
    parser.add_argument("-s", type=int, nargs=3, metavar=("MATCH", "MISMATCH", "GAP"), required=True, dest="scores", help="Scores for match, mismatch, and gap respectively")

    args = parser.parse_args()  # Parse the arguments

    return args.input_filepath, args.output_filepath, args.scores[0], args.scores[1], args.scores[2]


def read_sequences_from_fasta_and_lexicize(filepath: str) -> dict[str, str]:
    """
    Reads sequences from a FASTA file and store them in a dictionary.
    :param filepath: Path to the FASTA file.
    :return: Dictionary containing the sequence names as keys and the sequences as values.
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
    Main function to execute the UPGMA algorithm.
    It reads sequences from a FASTA file, builds a distance matrix, and constructs a UPGMA tree.
    Then it generates the Newick format of the tree and writes it to the output file with the given name.
    :return: None
    """
    input_filepath, output_filepath, match_score, mismatch_penalty, gap_penalty = parse_command_line_arguments()
    sequences = read_sequences_from_fasta_and_lexicize(input_filepath)

    upgma = UPGMA(sequences, match_score, mismatch_penalty, gap_penalty)
    _ = upgma.build()
    upgma.export_to_file(output_filepath)


if __name__ == "__main__":
    main()
