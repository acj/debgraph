-- Run FindCycles in a manner similar to CyclicTest in the C++ test suite
dofile('scripts/setup.l')
fc = FindCycles(g, {'CONTAINS','DEPENDS','PRE_DEPENDS','HAS_VERSION'}, 'Release:unstable')
print('Found ' .. #fc .. ' cycles')
for key,comp in pairs(fc) do      --"g" is the mother graph
	comp_nodes = ""
	if #comp > 0 then
		for nodekey,node in pairs(comp) do
			prop = GetProperty(node, 'Package')
			if prop ~= "OR" then
				comp_nodes = comp_nodes .. prop .. ' ' 
			end
		end
	end
	print('* ' .. comp_nodes)
end

