function WriteToLuaFile(arrNewKeyAndValue, arrOldKeyAndValue)
    print("here")

    local f

    local file = assert(io.open("Scripts/Experimental.lua", "r"))
    local fileContent = file:read("*all")
    file:close()

    print(fileContent)

    print(arrOldKeyAndValue)
    print(arrNewKeyAndValue)
    fileContent = string.gsub(fileContent, arrOldKeyAndValue, arrNewKeyAndValue)

    file = assert(io.open("Scripts/Experimental.lua", "w"))
    file:write(fileContent)
    file:close()
end