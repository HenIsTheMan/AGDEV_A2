---[[
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
	print("[LUA] GetBook("..index..")\n");
	return books[index];
end
--]]