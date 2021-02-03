---[[

function DistSquared(x0, y0, z0, x1, y1, z1, shldPrintResult)
     local distSquared = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + (z1 - z0) * (z1 - z0)

     if(shldPrintResult)
     then
        print("[LUA] DistSquared("..x0..", "..y0..", "..z0..", "..x1..", "..y1..", "..z1..") === "..distSquared.."\n")
     end

     return distSquared
end

--]]