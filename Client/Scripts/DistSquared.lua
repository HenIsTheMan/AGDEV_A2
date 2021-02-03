---[[

function DistSquared(x0, y0, z0, x1, y1, z1)
     local distSquared = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0)

     -- Uncomment to print
     -- print("[LUA] DistSquared("..x0..", "..y0..", "..z0..", "..x1..", "..y1..", "..z1..") === "..distSquared.."\n")

     return distSquared
end

--]]