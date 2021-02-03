---[[

function Perform(a, b)
	print("[LUA] Perform("..a..", "..b..")\n");
	return HostFunc(a + 7, b * 4);
end

--]]