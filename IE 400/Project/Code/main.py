"""
Bilkent University
Semester of Spring 2025

IE 400 Principles of Engineering Management
The Term Project

Date: May 11, 2025

Team 01:
    Kadir Hakan AKBAY 22102372
    Mısra Simge UYSAL 22201527
    Deniz Tuna ONGUNER 22001788
All the members are in the section "02".

This code is uploaded to the Department of Industrial Engineering at Bilkent University in partial
    fulfillment of the requirements for the course IE 400 Principles of Engineering Management.
"""

# -*- coding: utf-8 -*-
# Python Interpreter Version: 3.12.7

import ast  # for literal_eval
import numpy as np  # for dissimilarity calculation
import pandas as pd  # for data manipulation
import matplotlib.pyplot as plt  # for plotting
from gurobipy import Model, GRB, quicksum  # for optimization

# This dictionary defines the order of experience levels, where "Entry-level" is the lowest and "Manager" is the highest.
# The order is used to compare experience levels in the can_do function.
exp_order = {"Entry-level": 0, "Mid-level": 1, "Senior": 2, "Lead": 3, "Manager": 4}


def can_do(seeker, job, distance_matrix) -> bool:
    """
    Checks if a seeker can do a job, based on the given criteria:
        1. Job-type compatibility, i.e. the seeker’s desired job type must match the job type.
        2. Salary logic, i.e. the job's *max* salary must meet or exceed the seeker's minimum.
        3. Skills, i.e. the seeker’s skills must include all the required skills for the job.
        4. Experience, i.e. the seeker’s experience level must be equal to or higher than the job's required experience level.
        5. Location / commute, i.e. if the job is remote, the seeker can work from anywhere;
           otherwise the seeker must be within the maximum commute distance.
    :param seeker: The seeker to check whether they can do the job.
    :param job: The job to check whether the seeker can do it.
    :param distance_matrix: The distance matrix to check the commute distance.
    :return: True if the seeker can do the job, False otherwise.
    """
    global exp_order  # Use the global exp_order dictionary defined above

    # Job-type compatibility
    # The seeker’s desired job type must match the job type.
    if seeker.Desired_Job_Type != job.Job_Type:
        return False

    # Salary logic
    # Here we insist the job's *max* salary meets or exceeds the seeker's minimum.
    if job.Salary_Range_Max < seeker.Min_Desired_Salary:
        return False

    # Skills
    # The seeker's skills must include all the required skills for the job.
    seeker_sk = set(ast.literal_eval(seeker.Skills))
    req_sk = set(ast.literal_eval(job.Required_Skills))
    if not req_sk.issubset(seeker_sk):  # issubset() checks if all elements of req_sk are in seeker_sk
        return False

    # Experience
    # The seeker's experience level must be equal to or higher than the job's required experience level.
    if exp_order[seeker.Experience_Level] < exp_order[job.Required_Experience_Level]:
        return False

    # Location / commute
    # If the job is remote, the seeker can work from anywhere.
    # If the job is on-site or hybrid, the seeker must be within the maximum commute distance.
    if job.Is_Remote == 0:  # on-site / hybrid
        km = distance_matrix.loc[seeker.Location, job.Location]
        if km > seeker.Max_Commute_Distance:
            return False

    return True  # If all conditions are met, return True, meaning the seeker can do the job.


def main() -> None:
    """
    Main function to run the optimization model.
    This function reads data from CSV files, preprocesses the data, and solves two optimization problems:
        1. Maximize the total priority weight of job assignments.
        2. Minimize the maximum dissimilarity among job assignments while ensuring a minimum total priority weight.
    The results are printed and plotted.
    :return: None
    """
    # Read data from CSV files
    # The CSV files should be in the same directory as this script.
    seekers = pd.read_csv("seekers.csv")
    jobs = pd.read_csv("jobs.csv")
    distance_matrix = pd.read_csv("location_distances.csv", index_col=0)  # A-F matrix
    # Write the distance matrix to a CSV file as it is asked for.
    #distance_matrix.to_csv("distance_matrix.csv")  # Unnecessary, but can be used for debugging or analysis.

    # Preprocess data, time complexity is O(nm) where n is the number of seekers and m is the number of jobs.
    # Find all feasible seeker-job pairs
    pairs = []  # (seeker index, job index)
    for i, seeker in seekers.iterrows():  # Iterate over each seeker
        for j, job in jobs.iterrows():  # Iterate over each job
            if can_do(seeker, job, distance_matrix):  # Check if the seeker can do the job
                pairs.append((i, j))  # If they can, add the pair to the list

    print(f"{len(pairs)} feasible seeker–job pairs out of {len(seekers) * len(jobs)} total.")

    # ========================================= QUESTION 1 =========================================
    # Model 1: Maximize total priority weight
    m1 = Model("max_weight")
    x = m1.addVars(pairs, vtype=GRB.BINARY, name="x")

    # Constraints
    m1.addConstrs(
        # Each seeker can be assigned to at most one job
        (x.sum(i, '*') <= 1 for i in seekers.index),
        name='OneJobPerSeeker'
    )

    m1.addConstrs(
        # Each job can be assigned to at most its number of available positions
        (x.sum('*', j) <= jobs.at[j, 'Num_Positions'] for j in jobs.index),
        name='JobCapacity'
    )

    # Objective
    m1.setObjective(
        # The objective happens to be the sum of the priority weights of the jobs assigned to seekers
        sum(jobs.at[j, 'Priority_Weight'] * x[i, j] for i, j in pairs),
        GRB.MAXIMIZE  # Maximize the total priority weight
    )

    m1.optimize()  # Solve the model

    if m1.status == GRB.OPTIMAL:  # Check if the model found an optimal solution
        m_w = m1.ObjVal  # Maximum total priority weight
        print("Maximum total priority weight (Mw) =", m_w)
    else:  # If no optimal solution was found, print a message
        m_w = None
        print("No optimal solution found.")

    # ========================================= END QUESTION 1 =========================================

    # ========================================= QUESTION 2 =========================================
    # Compute dissimilarity dij for each feasible pair
    dij = {}
    for i, j in pairs:
        qi = np.array(ast.literal_eval(seekers.at[i, 'Questionnaire']), dtype=float)  # Convert string to array of floats
        qj = np.array(ast.literal_eval(jobs.at[j, 'Questionnaire']), dtype=float)  # Convert string to array of floats
        dij[(i, j)] = np.mean(np.abs(qi - qj))  # Calculate the dissimilarity, which is the mean of absolute differences

    # ω values to test
    omegas = [70, 75, 80, 85, 90, 95, 100]  # Values are given in the problem statement
    results = []  # Store results for plotting, cardinality of the results list will be equal to the number of ω values

    for omega in omegas:  # Iterate over each ω value
        m2 = Model(f"min_max_dij_{omega}")  # Create a new model for each ω value
        x2 = m2.addVars(pairs, vtype=GRB.BINARY, name="x2")  # Decision variables for this model, same as before

        # d_max variable: the maximum dissimilarity among the assigned pairs
        # This variable will be minimized in the objective function.
        # It is bounded between 0 and 5, as per the problem statement.
        #   i.e. lb -> "lower bound" and ub -> "upper bound"
        d_max = m2.addVar(lb=0.0, ub=5.0, name="d_max")

        # Constraints from Model 1
        m2.addConstrs(  # Each seeker can be assigned to at most one job
            (x2.sum(i, '*') <= 1 for i in seekers.index),
            name="OneJobPerSeeker"
        )

        m2.addConstrs(  # Each job can be assigned to at most its number of available positions
            (x2.sum('*', j) <= jobs.at[j, 'Num_Positions'] for j in jobs.index),
            name="JobCapacity"
        )

        # New constraints for this model
        # Ensure total priority weight ≥ ω% of Mw
        # This constraint ensures that the total priority weight of the assigned jobs
        #   is at least ω% of the maximum total priority weight (Mw).
        m2.addConstr(
            quicksum(jobs.at[j, 'Priority_Weight'] * x2[i, j] for i, j in pairs) >= (omega / 100.0) * m_w,
            name="MinWeight"
        )

        # d_max constraints
        # This constraint ensures that the maximum dissimilarity (d_max) is greater than or equal to
        #   the dissimilarity of the assigned pairs (dij).
        m2.addConstrs((d_max >= dij[(i, j)] * x2[i, j] for i, j in pairs), name='MaxDij')

        # Objective: minimize d_max
        m2.setObjective(d_max, GRB.MINIMIZE)

        m2.optimize()  # Solve the model

        if m2.status == GRB.OPTIMAL:  # Check if the model found an optimal solution
            results.append((omega, d_max.X))
            print(f"ω={omega}% → minimized maximum dij = {d_max.X}")
        else:
            results.append((omega, None))
            print(f"ω={omega}% → no optimal solution.")

    # Plotting the results
    omegas_plot, dvals = zip(*results)  # Unzip the results into two lists: omegas and dvals
    # omegas_plot contains the ω values and dvals contains the corresponding minimized maximum dissimilarities
    plt.figure()
    plt.plot(omegas_plot, dvals, marker='o')
    plt.xlabel('ω (%)')
    plt.ylabel('Minimized Maximum Dissimilarity')
    plt.title('Trade-off: ω% vs Max dij')
    plt.xticks(omegas_plot)
    plt.yticks(dvals)
    plt.grid(True, which='both', linestyle='--', linewidth=0.5)
    plt.show()

    # ========================================= END QUESTION 2 =========================================
    return None  # Explicitly return None to indicate the end of the function, i.e. good practice.


if __name__ == "__main__":
    # This ensures that the main function is called when the script is run directly.
    # May not be necessary for this assignment, but it's a good practice, hence included.
    main()
