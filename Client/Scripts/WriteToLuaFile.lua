---[[

function WriteOverwrite(fPath, newStr, oldStr)
    local file = assert(io.open(fPath, "r"))
    local fileContent = file:read("*all")
    file:close()

    fileContent = string.gsub(fileContent, oldStr, newStr)

    file = assert(io.open(fPath, "w"))
    file:write(fileContent)
    file:close()
end

--]]