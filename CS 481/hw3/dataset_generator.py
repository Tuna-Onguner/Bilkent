import random


class DatasetGenerator:
    class BaseDatasetGenerator:
        class Dataset:
            """
            A class representing a dataset of sequences.
            """

            def __init__(self, sequences: list[str]) -> None:
                """
                Initializes the Dataset with a list of sequences.
                """
                self.sequences = sequences
                self.sequences_count = len(self.sequences)
                self.sequence_length = len(self.sequences[0]) if self.sequences else 0

            def __repr__(self):
                """
                Returns a string representation of the Dataset.
                """
                return str(self.sequences)

        def __init__(self, alphabet: list[chr], make_sequence: callable) -> None:
            """
            Initializes the BaseDatasetGenerator with an alphabet and a sequence generation function.
            :param alphabet: The alphabet to use for generating sequences, "ACGT" by default.
            :param make_sequence: Function to generate a sequence of a given length.
            """
            self.alphabet = alphabet
            self.make_sequence = make_sequence

        def generate_dataset(self, number_sequences: int, length_sequences: int) -> Dataset:
            """
            Generates a dataset with a specified number of sequences and sequence length.
            :param number_sequences: The number of sequences to generate.
            :param length_sequences: The length of each sequence.
            :return: Dataset: A Dataset object containing the generated sequences.
            """
            return self.Dataset([self.make_sequence(length_sequences) for _ in range(number_sequences)])

        def generate_dataset_vary_sequence_number(self, number_datasets: int, length_sequences: int) -> list[Dataset]:
            """
            Generates datasets with varying numbers of sequences while keeping sequence lengths similar.
            :param number_datasets: Number of datasets to generate
            :param length_sequences: Length of each sequence in the datasets
            :return: List of generated datasets
            """
            return [self.generate_dataset(2 ** i, length_sequences) for i in range(number_datasets)]

        def generate_dataset_vary_sequence_length(self, number_datasets: int, number_sequences: int) -> list[Dataset]:
            """
            Generates datasets with varying sequence lengths while keeping the number of sequences similar.
            :param number_datasets: Number of datasets to generate
            :param number_sequences: Number of sequences in the datasets
            :return: List of generated datasets
            """
            return [self.generate_dataset(number_sequences, 2 ** i) for i in range(number_datasets)]

    def __init__(self, alphabet: list[chr] | None = None) -> None:
        """
        Initializes the RandomDatasetGenerator with a specified alphabet.
        :param alphabet: Alphabet to use for generating sequences (e.g., "ACGT" for DNA)
        """
        self.alphabet = list(alphabet) if alphabet is not None else ['A', 'C', 'G', 'T']

        def random_sequence(length: int) -> str:
            """
            Generates a random sequence of a given length using the specified alphabet.
            :param length: Length of the sequence
            :return: Randomly generated sequence
            """
            return "".join(random.choice(self.alphabet) for _ in range(length))

        def non_random_sequence(length: int) -> str:
            """
            Generates a non-random sequence of a given length using the specified alphabet.
            :param length: Length of the sequence
            :return: Non-randomly generated sequence
            """
            counter = getattr(non_random_sequence, 'counter', 0)
            sequence = "".join(self.alphabet[(counter + i) % len(self.alphabet)] for i in range(length))
            random_sequence.counter = counter + 1
            return sequence

        self.random = self.BaseDatasetGenerator(self.alphabet, random_sequence)
        self.non_random = self.BaseDatasetGenerator(self.alphabet, non_random_sequence)
