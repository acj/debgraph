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
if #arg > 0 then
	-- Use the given path as our output directory
	_OUTPUT_PATH = arg[1]
else
	_OUTPUT_PATH = '.'
end
libdg = package.loadlib('libdebgraph.so', 'luaopen_libdebgraph')
libdg()
print(ShowVersion())
print('Working directory: ' .. _OUTPUT_PATH)
LoadPackages('cache')

-- Run any scripts that are available
for i in io.popen('ls lua/tests/*.l'):lines() do
	dofile(i)
end
