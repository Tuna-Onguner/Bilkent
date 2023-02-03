#
# Bilkent University
# Fall 2022 - 2023
# CS315 Programing Languages
# Homework 1: Associative Arrays
# "Ruby Hash"
#
# 18.11.2022, Friday
#
# Section 2
# Deniz Tuna Onguner
# ID: 22001788
#
# References:
# https://www.tutorialspoint.com/ruby/ruby_hashes.htm
# https://www.codesdope.com/ruby-putsputsputs/
#


puts "CS315 Homework 1\nRuby Hash"

# Initialize a hash
discoveryYearsOfBosons = Hash["Higgs" => 2012, "Gluon" => 199, "Photon" => 1926]

# Print the hash
puts "\nHash of discovery years of some bosons: #{discoveryYearsOfBosons}"

# Element count of the hash
size = discoveryYearsOfBosons.length
puts "\nElement count of the hash is #{size}"

# Accessing elements
puts "\nThe higgs boson was discovered in #{discoveryYearsOfBosons["Higgs"]}"

# Modify an element
discoveryYearsOfBosons["Gluon"] = 1976

# Add a new element
discoveryYearsOfBosons["W"] = 1983
discoveryYearsOfBosons["Electron"] = 1897

# Print the updated hash
puts "\nHash updated: #{discoveryYearsOfBosons}"

# Remove an element
# Electron is not a boson
discoveryYearsOfBosons.delete("Electron")
puts "\nElectron is not a boson; it is removed: #{discoveryYearsOfBosons}"

# Search for a key
def searchInKeys(hash, key)
  for k in hash.keys; 
    return true if k == key
  end
  return false
end

puts "\nSerach for a key"
key = "W"
if searchInKeys(discoveryYearsOfBosons, key)
  puts "Key exits"
else
  puts "Key does not exist"
end

# Search for a value
def searchInValues(hash, val)
  for v in hash.values; 
    return true if v == val
  end
  return false
end

puts "\nSerach for a value"
val = 2012
if searchInValues(discoveryYearsOfBosons, val)
  puts "Value exits"
else
  puts "Value does not exist"
end

# foo function
def foo(hash)
  hash.each { |key, val| puts "#{key} => #{val}\n" }
end

puts "\nfoo:\n"
foo(discoveryYearsOfBosons)

# Clear a hash
discoveryYearsOfBosons.clear
puts "\nHash is cleared, new hash: #{discoveryYearsOfBosons}"

# Nested hash
discoveryYearsOfParticles = Hash[
  "Bosons" => Hash["Higgs" => 2012, "Gluon" => 1997, "Photon" => 1926],
  "Fermions" => Hash["Electron" => 1897, "Muon" => 1936, "Tau" => 1975]
]

puts "\nNested hash: #{discoveryYearsOfParticles}"
puts "\nFoo function for nested hash:"

foo(discoveryYearsOfParticles)

# Add a new element
discoveryYearsOfParticles["Bosons"]["W"] = 1983
discoveryYearsOfParticles["Fermions"]["Proton"] = 1917

puts "\nUpadted nested hash: #{discoveryYearsOfParticles}"

# Remove an element
# Proton is not a fermion
discoveryYearsOfParticles["Fermions"].delete("Proton")

# Remove an inner hash
discoveryYearsOfParticles.delete("Bosons")

puts "\nFinal nested hash: #{discoveryYearsOfParticles}"

# End File
