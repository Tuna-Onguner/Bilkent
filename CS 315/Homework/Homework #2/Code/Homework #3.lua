--[[
 -
 - Bilkent University
 - Fall 2022 - 2023
 -
 - CS315 Programing Languages
 - Homework 2: User-Located Loop Control Mechanisms
 - Language: Lua, version: 5.4.4
 -
 - 03.12.2022, Saturday
 -
 - Section 2
 - Deniz Tuna Onguner
 - ID: 22001788
 -
 - References:
 - https://www.tutorialspoint.com/lua/lua_loops.htm
 - https://www.lua.org/pil/4.3.4.html
--]]

print("CS315 Homework 2\nUser-Located Loop Control Mechanisms in Lua\n")

var = 0

-- Example 1
while (true) do
    var = var + 1
    if var >= 11 then 
        break 
    end
end

print("Output(Example 1):", var)

var = 0

-- Example 2
repeat
    var = var + 1
    while (true) do 
        var = var * 10
        if var >= 100 then
            break
        end
    end
until (var >= 200)

print("Output(Example 2):", var)

var = 0

-- Example 3
while (true) do
    while (true) do
        var = var + 1
        if var > 10 then
            goto exitOuterLoop
        elseif var > 5 then
            goto exitInnerLoop
        end
    end 
    ::exitInnerLoop::
    var = var * 10
end
::exitOuterLoop::

print("Output(Example 3):", var)


-- End of the file
