import numpy as np
import matplotlib.pyplot as plt
import os, time, tracemalloc, psutil
from collections import defaultdict
from dataset_generator import DatasetGenerator
from center_star_alignment import center_star_alignment

"""
This script is to analyze the performance of the center star alignment algorithm and to plot the execution time 
against the dataset size and sequence length for reporting purposes, making it independent from the main program.
Hence, it does not include the command line argument parsing or file reading/writing, and also it does utilize
external libraries such as numpy and matplotlib for plotting.
"""


def plot_metric(results: list[dict[str, float]], x_key: str, y_key: str, group_key: str, x_label: str, y_label: str,
                group_label: str, match: int, mismatch: int, opening: int, gap: int, ref_curve: bool = False) -> None:
    """
    This function plots the execution time of the center star alignment algorithm against the dataset size and/or sequence length.
    :param results: List of dictionaries containing dataset size, sequence length, and execution time
    :param x_key: The key to use for the x-axis (dataset size or sequence length)
    :param y_key: The key to use for the y-axis (execution time)
    :param group_key: The key to group the results by (dataset size or sequence length)
    :param x_label: The label for the x-axis
    :param y_label: The label for the y-axis
    :param group_label: The label for the grouped data
    :param ref_curve: Whether to plot a reference curve (default is False)
    :param match: Match score
    :param mismatch: Mismatch penalty
    :param opening: Opening penalty
    :param gap: Gap penalty
    :return: None
    """
    grouped = defaultdict(list)
    for r in results:
        grouped[r[group_key]].append((r[x_key], r[y_key]))

    plt.figure(figsize=(10, 6))
    all_x = []

    for gval, pts in grouped.items():
        pts.sort()
        xs, ys = zip(*pts)
        all_x.extend(xs)
        plt.plot(xs, ys, marker='o', label=f'{group_label}: {gval}')

    if ref_curve:
        xs = np.linspace(min(all_x), max(all_x), 100)
        ys = xs ** 2
        # scale to same y‑range for visual comparison
        scale = max(p[1] for pts in grouped.values() for p in pts) / max(ys)
        plt.plot(xs, ys * scale, '--', label='reference$x^2$')

    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(f'{y_label} vs. {x_label}\n(match={match}, mismatch={mismatch}, '
              f'opening={opening}, gap={gap})')
    plt.legend(title=group_label)
    plt.grid(True)
    plt.tight_layout()
    plt.show()


def run_test_case(datasets: list[DatasetGenerator.BaseDatasetGenerator.Dataset], *args) -> list[dict[str, int | float]]:
    """
    This function runs the center star alignment algorithm on a list of datasets and measures the execution time.
    :param datasets: List of datasets to run the algorithm on
    :param args: Arguments for the center star alignment function
    :return: List of dictionaries containing dataset size, sequence length, and execution time
    """
    results = []

    for dataset in datasets:  # Iterate through each dataset
        # --- time + RSS before the call ------------------------------------
        rss_before = psutil.Process(os.getpid()).memory_info().rss
        tracemalloc.start()
        tic = time.perf_counter()  # Start the timer

        _ = center_star_alignment(dataset.sequences, *args)

        # --- time + RSS after the call -------------------------------------
        toc = time.perf_counter()  # Stop the timer
        peak_by = tracemalloc.get_traced_memory()[1]
        tracemalloc.stop()
        rss_after = psutil.Process(os.getpid()).memory_info().rss

        results.append({
            "dataset_size": dataset.sequences_count,
            "sequence_length": dataset.sequence_length,
            "exec_time_s": toc - tic,
            "rss_before_mb": rss_before / 1024 ** 2,
            "rss_after_mb": rss_after / 1024 ** 2,
            "rss_delta_mb": (rss_after - rss_before) / 1024 ** 2,  # net change
            "peak_py_mb": peak_by / 1024 ** 2
        })

    return results


if __name__ == '__main__':
    def main() -> None:
        """
        This is the main function to analyze the performance of the center star alignment algorithm.
        It generates datasets with varying sizes and sequence lengths, runs the alignment algorithm,
            and plots the execution time against the dataset size and sequence length.
        :return: None
        """
        match, mismatch, opening, gap = 1, -1, -2, -1  # Arbitrary scoring scheme, change as needed

        dg = DatasetGenerator()  # Instance to generate random datasets
        # dg = RandomDatasetGenerator(alphabet=<YOUR_ALPHABET>)  # Change alphabet if needed
        # The default alphabet is 'ACGT' as this course is Bioinformatics Algorithms

        datasets_size = 6  # The designated number of datasets to generate
        # Be careful with the number of datasets, as it grows exponentially

        # TEST CASE 1: Vary the number of sequences while keeping sequence lengths similar
        length_sequences = 100  # The designated length of each sequence in the datasets
        datasets = dg.non_random.generate_dataset_vary_sequence_number(datasets_size, length_sequences)
        results = run_test_case(datasets, match, mismatch, opening, gap)
        # execution‑time plot (what you already had)
        plot_metric(results,
                    x_key='dataset_size', y_key='exec_time_s',
                    group_key='sequence_length',
                    x_label='Dataset size', y_label='Execution time (s)',
                    group_label='Sequence length',
                    match=match, mismatch=mismatch, opening=opening, gap=gap,
                    ref_curve=True)

        # peak Python‑level memory
        plot_metric(results,
                    x_key='dataset_size', y_key='peak_py_mb',
                    group_key='sequence_length',
                    x_label='Dataset size', y_label='Peak Python memory (MB)',
                    group_label='Sequence length',
                    match=match, mismatch=mismatch, opening=opening, gap=gap)

        # net resident‑set increase
        plot_metric(results,
                    x_key='dataset_size', y_key='rss_delta_mb',
                    group_key='sequence_length',
                    x_label='Dataset size', y_label='RSS after call (MB)',
                    group_label='Sequence length',
                    match=match, mismatch=mismatch, opening=opening, gap=gap)

        # TEST CASE 2: Vary the sequence lengths while keeping the number of sequences similar
        number_sequences = 32  # The designated number of sequences in the datasets
        datasets = dg.non_random.generate_dataset_vary_sequence_length(datasets_size, number_sequences)
        results = run_test_case(datasets, match, mismatch, opening, gap)
        plot_metric(results,
                    x_key='sequence_length', y_key='exec_time_s',
                    group_key='dataset_size',
                    x_label='Sequence length', y_label='Execution time (s)',
                    group_label='Dataset size',
                    match=match, mismatch=mismatch, opening=opening, gap=gap,
                    ref_curve=True)

        # peak Python‑level memory
        plot_metric(results,
                    x_key='sequence_length', y_key='peak_py_mb',
                    group_key='dataset_size',
                    x_label='Sequence length', y_label='Peak Python memory (MB)',
                    group_label='Dataset size',
                    match=match, mismatch=mismatch, opening=opening, gap=gap)

        # net resident‑set increase
        plot_metric(results,
                    x_key='sequence_length', y_key='rss_delta_mb',
                    group_key='dataset_size',
                    x_label='Sequence length', y_label='RSS after call (MB)',
                    group_label='Dataset size',
                    match=match, mismatch=mismatch, opening=opening, gap=gap)


    main()
