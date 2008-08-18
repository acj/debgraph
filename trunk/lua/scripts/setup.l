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
	_PATH = arg[1]
else
	_PATH = '.'
end
libdg = package.loadlib('libdebgraph.so', 'luaopen_libdebgraph')
libdg()
print(ShowVersion())
LoadPackages(_PATH .. '/cache')

-- Run any scripts that are available
for i in io.popen('ls /usr/share/debgraph/tests/*.l'):lines() do
	dofile(i)
end
