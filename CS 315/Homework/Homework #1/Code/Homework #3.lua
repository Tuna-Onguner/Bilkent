--[[
 - Bilkent University
 - Fall 2022 - 2023
 - CS315 Programing Languages
 - Homework 1: Associative Arrays
 - "Tables of Lua"
 -
 - 18.11.2022, Friday
 -
 - Section 2
 - Deniz Tuna Onguner
 - ID: 22001788
 -
 - References:
 - https://www.tutorialspoint.com/lua/lua_tables.htm
--]]


print ("CS315 Homework 1\nTables of Lua")

-- Initialize a table
BirthYearsOfPLs = {}
BirthYearsOfPLs["Java"] = 1895
BirthYearsOfPLs["Lua"] = 1993
BirthYearsOfPLs["C++"] = 1983
BirthYearsOfPLs["Dart"] = 2011

-- Print the table
print ("\nPrinting table:")
for key, val in pairs(BirthYearsOfPLs) do
  print (key," => ",val)
end

-- Element count of the table
Count = 0
for _ in pairs(BirthYearsOfPLs) do
  Count = Count + 1
end
print ("\nSize of the table is", Count)

-- Modify an element
BirthYearsOfPLs["Java"] = 1995

-- Remove an element
-- Setting the key that is desired to,
-- remove to nil is the way to remove it
BirthYearsOfPLs["Lua"] = nil

print ("\nPrinting updated table:")
for key, val in pairs(BirthYearsOfPLs) do
  print (key," => ",val)
end

-- Search in keys
function SearchInKeys(table, key)
  for k, _ in pairs(table) do
    if k == key then
      return true
    end
  end
  return false
end

-- Search in values
function SearchInVals(table, val)
  for _, v in pairs(table) do
    if v == val then
      return true
    end
  end
  return false
end

-- Search in keys
print ("\nSearch for a key")
Key = "C++"
if SearchInKeys(BirthYearsOfPLs, Key) then
 print ("Key exist")
else
  print ("Key does not exist")
end

-- Search in values
print ("\nSearch for a valur")
Val = 2012
if SearchInVals(BirthYearsOfPLs, Val) then
 print ("Value exist")
else
  print ("Value does not exist")
end

-- foo function
print ("\nfoo function")
function Foo(table)
  print ("Table: {")
  for key, val in pairs(table) do
    print (key," => ",val)
  end
  print ("}")
end

Foo(BirthYearsOfPLs)

-- Clear table
for key, _ in pairs(BirthYearsOfPLs) do
  BirthYearsOfPLs[key] = nil
end

-- Now call foo again
print ("\nCleared table")
Foo(BirthYearsOfPLs)

-- End of file

