require('Scripts.Terrain')

wayptPos = {}

---[[
function GenWayptPos(qty)
	print(qty)

	temp = {terrainXScale * 0.1 * (i - qty / 2), terrainYScale, terrainZScale * 0.1 * (i - qty / 2)}

	for i = 1, qty do
		table.insert(wayptPos, temp)
	end
end
--]]

--[[
local function IGenWayptPos(qty)
	temp = {}

	for i = 1, qty do
		table.insert(temp, i, {terrainXScale * 0.1 * (i - qty / 2), terrainYScale, terrainZScale * 0.1 * (i - qty / 2)})
	end

	return temp
end

function GenWayptPos(qty)
	wayptPos.init = IGenWayptPos(qty)
end
--]]