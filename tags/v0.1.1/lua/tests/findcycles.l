if _OUTPUT_PATH == nil then
	print('Please run setup.l first!')
	return
end

-- CyclesToHTML(file f, table cycles, string dist)
function CyclesToHTML (f, cycles, dist)
	f:write("<h2>Cyclic Depends (" .. dist .. ")</h2>\n")
	if #cycles == 0 then 
		f:write('No cycles found!')	
	end
	for key,graph in pairs(cycles) do
		local comp = GetNodes(graph)
		local comp_nodes = ""
		if #comp > 0 then
			for nodekey,node in pairs(comp) do
				--Pay attention to nodes only (ignore table members)
				if type(node) == "table" then
					local prop = GetProperty(node, 'Package')
					if prop ~= "OR" then
						comp_nodes = comp_nodes .. prop .. ' ' 
					end
				end
			end
			local pngFilename = 'dot/' .. dist .. '-' ..  GetProperty(comp[1], "Package") .. '.png'
			f:write('* ' .. '<a href=\'' .. pngFilename .. '\'>' ..  comp_nodes .. '</a><br/>\n')
			local dotFilename = _OUTPUT_PATH .. '/dot/' .. dist .. '-' ..  GetProperty(comp[1], "Package") .. '.dot'
			ToGraphviz(graph, dotFilename)
		end
	end
end

-- Run FindCycles in a manner similar to CyclicTest in the C++ test suite
local f = assert(io.open(_OUTPUT_PATH .. '/cyclictest.html', 'w'))

-- Set up HTML headers
f:write("<html>\n<body>\n<h1>CYCLICTEST</h1>\n")

fc = FindCycles(g, {'CONTAINS','DEPENDS','PRE_DEPENDS','HAS_VERSION'}, 'Release:unstable')
print('Found ' .. #fc .. ' cycles in distribution \'unstable\'')
CyclesToHTML(f, fc, 'unstable')

fc = FindCycles(g, {'CONTAINS','DEPENDS','PRE_DEPENDS','HAS_VERSION'}, 'Release:stable')
print('Found ' .. #fc .. ' cycles in distribution \'stable\'')
CyclesToHTML(f, fc, 'stable')

fc = FindCycles(g, {'CONTAINS','DEPENDS','PRE_DEPENDS','HAS_VERSION'}, 'Release:testing')
print('Found ' .. #fc .. ' cycles in distribution \'testing\'')
CyclesToHTML(f, fc, 'testing')

-- Clean up
f:write('\n</body></html>')
f:close()
