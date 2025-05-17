class Phylip:
    """
    Class to handle the PHYLIP format for multiple sequence alignment.
    The PHYLIP format is a simple text format for representing multiple sequence alignments.
    """

    def __init__(self, max_seq_name_length: int = 10) -> None:
        """
        Initialize the Phylip class with a maximum sequence name length.
        :param max_seq_name_length: Maximum length of the sequence names, 10 by default.
        """
        self.max_seq_name_length = max_seq_name_length

    def write_phylip(self, filename: str, sequences: dict[str, str]) -> None:
        """
        Write sequences to a PHYLIP file.
        :param filename: Name of the output file.
        :param sequences: List of sequences to write.
        """
        sequence_number = len(sequences)
        sequence_length = len(next(iter(sequences.values())))

        with open(filename, "w", encoding="utf-8") as phylip_file:
            """
            From the conversation with ChatGPT o3 on Apr 18 2025 - About the PHYLIP header line:
                “Use spaces, not TABs.
                Strict PHYLIP requires the two integers on the header line to be separated by ≥1 ordinary blank space;
                extra leading or internal blanks are tolerated, but TAB characters are outside the spec and can break some parsers.” [1]
            Additionally, the webpage 'phylo.org' agrees with this statement.
                “Line number 1 provides number of taxa, then one space (Tab not allowed), then the number of characters 
                in each taxon (each taxon has the same number of characters).” [2]
            Hence, I decided to follow this convention.
            
            [1] OpenAI ChatGPT, “Discussion of PHYLIP header‑line whitespace, private communication, Apr. 8, 2025.
            [2] Phylip format. CIPRES. (n.d.). https://www.phylo.org/index.php/help/phylip
            """
            phylip_file.write(f"{sequence_number} {sequence_length}\n")

            for name, sequence in sequences.items():
                # Ensure the sequence name is padded to the maximum length
                padded_name = name.ljust(self.max_seq_name_length)
                phylip_file.write(f"{padded_name} {sequence}\n")
