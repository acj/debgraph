-- Supporting functions
function ObjectDump (obj)
	if type(obj) == 'table' then
		print('key\t\tvalue')
		for key,val in pairs(obj) do
			print(key .. '\t\t' .. val)
		end
	end
end

-- Basic setup: load the shared library and the contents of "cache"
libdg = package.loadlib('libdebgraph.so', 'luaopen_libdebgraph')
libdg()
print(ShowVersion())
LoadPackages('cache')
