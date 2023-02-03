#
# Bilkent University
# Fall 2022 - 2023
#
# CS315 Programing Languages
# Homework 2: User-Located Loop Control Mechanisms
# Language: Ruby, version: 2.6.10
#
# 03.12.2022, Saturday
#
# Section 2
# Deniz Tuna Onguner
# ID: 22001788
#
# References:
# https://www.geeksforgeeks.org/ruby-loops-for-while-do-while-until/
# https://www.geeksforgeeks.org/ruby-break-and-next-statement/
#

puts "CS315 Homework 2\nUser-Located Loop Control Mechanisms in Ruby\n"

var = 0

# Example 1
while true
    var += 1
    if var == 8
        break
    end
end

puts "\nOutput of Example 1:  #{var}"

var = 0

# Example 2
for i in 0..10
    if i % 3 == 0
        next
    end
    var = var + i
end

puts "Output of Example 2:  #{var}"

var = 0

# Example 3
while true do
    while true do
        if var == 10
            break
        end

        var += 1
    end

    break
end

puts "Output of Example 3:  #{var}"

var = 0

# End of the file
