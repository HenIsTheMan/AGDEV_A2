require('Scripts.Terrain')

wayptPos = {}

local rows = 9
local cols = 18

for r = 0, rows do
	for c = 0, cols do
		table.insert(wayptPos, r * cols + c + 1, {-terrainXScale * 0.45 + terrainXScale * 0.05 * c, terrainYScale * 0.8, terrainZScale * 0.05 * r})
	end
end

function AmtOfWaypts()
	return #wayptPos
end