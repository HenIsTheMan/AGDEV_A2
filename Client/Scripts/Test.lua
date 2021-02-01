-- Test.lua

--[[
a = 40 + math.sin(40.5)
a = a + 400 * 4
--]]

--[[
foobar = {}
foobar["myStr"] = "Hi"
foobar["myDouble"] = 4.0
--]]

--[[
foobar = {
	myStr = "Hi",
	myDouble = 4.0
}
--]]

function DoSth(a, b)
	print("[LUA] DoSth("..a..", "..b..")\n");
	return a * b;
end

--[[
name = "Esteban"
local name = "Henry"

val = 40
local val = 4.0
--]]