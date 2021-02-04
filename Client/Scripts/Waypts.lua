require('Scripts.Terrain')

wayptPos = {}

local rows = 9
local cols = 18

for r = 0, rows do
	for c = 0, cols do
		table.insert(wayptPos, r * cols + c + 1, {-terrainXScale * 0.45 + terrainXScale * 0.05 * c, terrainYScale * 0.8, terrainZScale * 0.05 * r})
	end
end

-- wayptPos = shuffle(wayptPos)

function AmtOfWaypts()
	return #wayptPos
end

function shuffle(arr) -- Fisher-Yates shuffle
	for i = #arr, 2, -1 do
		local j = math.random(i)
		arr[i], arr[j] = arr[j], arr[i]
	end
	return arr
end