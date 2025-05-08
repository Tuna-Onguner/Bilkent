from itertools import combinations  # For generating all unique pairs of sequences
from collections import defaultdict  # For creating a nested dictionary for the distance matrix
from needleman_wunsch import needleman_wunsch  # Importing the Needleman-Wunsch algorithm for sequence alignment

"""
In addition to the course slides, following GitHub repositories are applied for the implementation of UPGMA and Newick format:
- thaze, BioInformatics – concise UPGMA prototype in plain Python, GitHub repository, https://github.com/thaze/BioInformatics [accessed 30 Apr 2025]
- aeyc, UPGMA – example coupling of Needleman-Wunsch with UPGMA and Newick output, GitHub repository, https://github.com/aeyc/UPGMA [accessed 30 Apr 2025]
- lex8erna, UPGMApy – clear exposition of the average-linkage distance-update rule, GitHub repository, https://github.com/lex8erna/UPGMApy [accessed 30 Apr 2025]

The following webpage is applied for newick format during the implementation:
- J. Felsenstein, “The Newick tree format,” PHYLIP Web Pages, https://phylipweb.github.io/phylip/newicktree.html [accessed 1 May 2025]

Those sources above are also listed at the end of the README file.
"""


class UPGMANode:
    """
    A node in the UPGMA tree.
    Each node represents a cluster of sequences.
    It contains the name of the cluster, pointers to its left and right children,
        the height of the node, and the size of the cluster.

    Note:
        The instance of this class are not private, violating the encapsulation principle of OOP.
        However, this is intentional for ease of use and to reduce the complexity of the code.
        One can add getters and setters for these attributes if needed.
    """

    def __init__(self, name: str = "") -> None:
        """
        Initialize a UPGMA node.
        :param name: Name of the cluster, which will be exported in Newick format as the node name.
            Empty string by default, for internal nodes, change as needed.
        :return: None
        """
        self.name = name
        self.left = None
        self.right = None
        self.height = 0.0
        self.size = 1

    def generate_newick_format_recursive(self) -> str:
        """
        Convert the UPGMA tree to Newick format recursively.
        Call from the root node of the tree.
        :return: The Newick format string representing the tree.

        Note:
            This function is called recursively for each node in the tree.
            In-order traversal is used to generate the Newick format.
        """
        if not self.left and not self.right:  # no children, leaf node
            return self.name

        # Recursively call for left and right children
        left_str = self.left.generate_newick_format_recursive() + f":{self.height - self.left.height:.5f}"
        right_str = self.right.generate_newick_format_recursive() + f":{self.height - self.right.height:.5f}"

        return f"({left_str},{right_str})"  # Return the Newick format string for the current node


class UPGMA:
    """
    UPGMA Tree construction class for phylogenetic analysis.
    This class builds a UPGMA tree from a distance matrix and a set of sequences.
    It uses the UPGMA algorithm to iteratively merge the closest clusters until only one cluster remains.
    The final tree is represented as a UPGMANode object, i.e., the root of the tree.
    Unline Node class, OOP principles are not violated in this class.
    """

    def __init__(self, sequences: dict[str, str], match_score: int, mismatch_penalty: int, gap_penalty: int) -> None:
        """
        Initialize the UPGMA object from a set of sequences and their scoring scheme.
        :param sequences: Dictionary of sequences where keys are sequence names and values are the sequences.
        :param match_score: Score for matching characters in the alignment.
        :param mismatch_penalty: Penalty for mismatching characters in the alignment.
        :param gap_penalty: Penalty for introducing gaps in the alignment.
        :return: None

        Note:
            This function is to initialize the UPGMA object, to build the tree, call the build() method after this.
        """
        self.__distance_matrix = self.__build_distance_matrix(sequences, match_score, mismatch_penalty, gap_penalty)
        # This nodes dictionary below will be used to keep track of the nodes in the tree
        self.__nodes = {name: UPGMANode(name) for name in sequences.keys()}
        self.__root = None  # The root node of the UPGMA tree, will be set in the build() method, None initially

    @staticmethod
    def __compute_distance(aligned_query: str, aligned_subject: str) -> int:
        """
        Compute the distance between two aligned sequences.
        :param aligned_query: Query sequence.
        :param aligned_subject: Subject sequence.
        :return: The distance between the two sequences.

        Note:
            'distance = number of mismatches + number of gaps'
            as defined in the assignment description.
        """
        mismatches = sum(1 for a, b in zip(aligned_query, aligned_subject) if a != b)  # Count mismatches
        gaps = sum(1 for a in aligned_query if a == '-') + sum(1 for b in aligned_subject if b == '-')
        return mismatches + gaps  # Return the total distance

    @staticmethod
    def __build_distance_matrix(sequences: dict[str, str], match: int, mismatch: int, gap: int) -> dict[str, dict[str, float]]:
        """
        Build the distance matrix for the given sequences using the Needleman-Wunsch algorithm.
        :param sequences: Dictionary of sequences to build the distance matrix for.
        :param match: Score for matching characters in the alignment.
        :param mismatch: Penalty for mismatching characters in the alignment.
        :param gap: Penalty for introducing gaps in the alignment.
        :return: A nested dictionary representing the distance matrix.

        Note:
            The distance matrix created is a symmetric matrix.
            The diagonal elements are None, as the distance between a sequence and itself is not used in UPGMA.
        """

        distance_matrix = defaultdict(dict)  # Using defaultdict to create a nested dictionary
        # Nested dictionary is needed for symmetric distance matrix
        # A non-symmetric matrix can also be used, but I prefer to keep it symmetric for clarity and ease of use

        for (query_name, query_sequence), (subject_name, subject_sequence) in combinations(sequences.items(), 2):
            # combinations(sequences.items(), 2) generates all unique pairs of sequences
            aligned_query, aligned_subject, _ = needleman_wunsch(query_sequence, subject_sequence, match, mismatch, gap)
            # score itself is not needed, only the aligned sequences are enough
            distance = UPGMA.__compute_distance(aligned_query, aligned_subject)

            # Set both directions to maintain symmetry
            if query_name not in distance_matrix:
                distance_matrix[query_name] = {}  # Set the inner dictionary if it doesn't exist already
            if subject_name not in distance_matrix:
                distance_matrix[subject_name] = {}

            distance_matrix[query_name][subject_name] = distance  # Distance from query to subject
            distance_matrix[subject_name][query_name] = distance  # Mirrors from subject to query, symmetric matrix

        return distance_matrix

    def build(self) -> UPGMANode:
        """
        Build the UPGMA tree using the distance matrix.
        The tree is built by iteratively merging the closest clusters until only one cluster remains.
        :return: The root node of the UPGMA tree.
        :raises: ValueError if the tree cannot be built due to invalid distance matrix.
        """
        # Initialize clusters with each sequence as a separate cluster
        clusters = {name: [name] for name in self.__nodes.keys()}

        while len(clusters) > 1:
            # Find the closest pair of clusters
            min_dist = float('inf')  # This syntax grantees that some_variable < float('inf') is always True
            closest_pair = (None, None)

            for name1 in clusters:  # Iterate over all clusters
                for name2 in clusters:
                    if name1 == name2:  # Skip if the same cluster
                        continue

                    if name2 in self.__distance_matrix[name1]:
                        distance_current = self.__distance_matrix[name1][name2]
                        if distance_current < min_dist:
                            min_dist = distance_current
                            closest_pair = (name1, name2)

            name1, name2 = closest_pair

            # If no valid pairs found, raise an error (Debugging purposes)
            if name1 is None or name2 is None:
                raise ValueError("No valid pairs found to merge. Something went wrong.")

            # Merge clusters
            new_cluster_name = name1 + '+' + name2  # New cluster name, can be changed as needed
            new_node = UPGMANode(new_cluster_name)  # Create a new node for the merged cluster
            new_node.left = self.__nodes[name1]  # Set left child to the first cluster
            new_node.right = self.__nodes[name2]  # Set right child to the second cluster
            new_node.size = self.__nodes[name1].size + self.__nodes[name2].size  # Update size of the new cluster
            new_node.height = min_dist / 2.0  # Set height of the new node to half the distance between the two clusters

            # Register the new node
            self.__nodes[new_cluster_name] = new_node

            # Add the new cluster to clusters
            clusters[new_cluster_name] = clusters.pop(name1) + clusters.pop(name2)

            # Update distance matrix for the new cluster
            self.__distance_matrix[new_cluster_name] = {}

            # Calculate distances to another cluster clusters
            for another_cluster in clusters:
                if another_cluster == new_cluster_name:  # Skip if the same cluster
                    continue

                # Calculate the distance to the new cluster
                distance1_to_another = self.__distance_matrix[name1][another_cluster]
                distance2_to_another = self.__distance_matrix[name2][another_cluster]

                # The distance is the weighted average of the distances to the two merged clusters
                new_distance = ((distance1_to_another * self.__nodes[name1].size +
                                 distance2_to_another * self.__nodes[name2].size) / new_node.size)

                # Update symmetrically, i.e., the distance matrix is symmetric
                self.__distance_matrix[new_cluster_name][another_cluster] = new_distance
                self.__distance_matrix[another_cluster][new_cluster_name] = new_distance

            # Clean up old clusters from distance matrix
            del self.__distance_matrix[name1]
            del self.__distance_matrix[name2]

            for another_cluster in self.__distance_matrix:
                self.__distance_matrix[another_cluster].pop(name1, None)
                self.__distance_matrix[another_cluster].pop(name2, None)

        # Only one cluster remains: return its node
        self.__root = self.__nodes[list(clusters.keys())[0]]  # The root node of the UPGMA tree
        return self.__root

    def generate_newick_format(self) -> str:
        """
        Generate the Newick format string for the UPGMA tree.
        :return: The Newick format string representing the UPGMA tree.
        :raises: ValueError if the UPGMA tree is empty.
        """
        if self.__root is None:
            raise ValueError("The tree has not been built yet. Call build() before generating Newick format.")

        return self.__root.generate_newick_format_recursive() + ';'  # Semicolon at the end of Newick format is required

    def export_to_file(self, filepath: str) -> None:
        """
        Export the UPGMA tree to a file in Newick format.
        :param filepath: Path to the output file.
        :return: None
        :raises: IOError if the file cannot be written for any reason.
        """
        tree_in_newick_format = self.generate_newick_format()

        try:
            with open(filepath, 'w', encoding="utf-8") as export_file:
                export_file.write(tree_in_newick_format)
        except IOError as e:
            print(f"Error writing to file {filepath}: {e}")
            raise e
