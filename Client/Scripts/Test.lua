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

--[[
function DoSth(a, b)
	print("[LUA] DoSth("..a..", "..b..")\n");
	return a * b;
end
--]]

books = {};
books[0] = {
	myStr = "Hey",
	myDouble = 7.5
}
books[1] = {
	myStr = "Ho",
	myDouble = 50.22
}

function GetBook(index)
	print("[LUA] DoSth("..index..")\n");
	return books[index];
end

--[[
name = "Esteban"
local name = "Henry"

val = 40
local val = 4.0
--]]