#
# Bilkent University
# Fall 2022 - 2023
#
# CS315 Programing Languages
# Homework 2: User-Located Loop Control Mechanisms
# Language: Python, version: 3.9
#
# 03.12.2022, Saturday
#
# Section 2
# Deniz Tuna Onguner
# ID: 22001788
#
# References:
# https://docs.python.org/3/tutorial/controlflow.html
#

print("CS315 Homework 2\nUser-Located "
      "Loop Control Mechanisms in Python\n")

# Example 1
var = 0
while True:
    var += 1
    if var >= 10:
        break

print(f"Ex. 1:  var = {var}")

# break can be used to exit a loop,
# which is unconditional and unlabeled.

# Example 2
var = 0
while True:
    var += 1
    break

print(f"Ex. 2:  var = {var}")

# Example 3
var = 0
while True:
    while True:
        var += 1
        if var >= 20:
            break
    if var >= 20:
        break

print(f"Ex. 3:  var = {var}")

# Since break is unlabeled, multiple breaks
# are required to exit a nexted loop in Python.
# Which means the exact same code block must repeat.

# Example 4
var = 0
for i in range(10):
    if i == 5:
        continue
    var += i

print(f"Ex. 4:  var = {var}")

# continue will pass the next codes after it,
# and start the next loop execution.

# Example 5
var = 0
for i in range(5):
    if i >= 3:
        continue
    for j in range(10):
        if j == 5:
            continue
        var += j

print(f"Ex. 5:  var = {var}")

# Similar to break, continue is also unlabeled.
# Which means nested loops also requires multiple continues.

# Example 6
var = 0
for i in range(5):
    for j in range(5):
        var += 1
        if var == 3:
            break
    else:
        var = -10

print(f"Ex. 6:  var = {var}")

# else can be used with loops in Python.

del var
# End of the file
