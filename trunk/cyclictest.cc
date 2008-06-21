#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "cyclictest.h"
#include "filter.h"
#include "findcycles.h"
#include "finddeps.h"

bool CyclicTest::run(Graph &g) {
	FindCycles fc(g, FindCycles::DEPENDS, "Release:unstable");
	fc.execute();
	vector<Graph>& cycles = fc.getCycles();
	cout << "\n\tFound " << cycles.size() << endl;
	char cycleName[35];
	// Set up a filter rule for BINARYNAME only to reduce the graph
	FilterProperties fp;
	FilterRule fRule = { string("Type"), EQUALS, string("BINARYNAME") };
	fp.push_back(fRule);
	for (size_t i = 0; i < cycles.size(); ++i) {
		Graph reducedCycle = Filter(cycles[i], fp, FILTER_AND).execute();
		// Write out the original
		sprintf(cycleName, "out/cyclictest-orig-%d.dot", i);
		ofstream dotfile_orig(cycleName);
		dotfile_orig << cycles[i].toGraphviz();
		dotfile_orig.close();
		// Write out the reduced (BINARYNAME only) version
		sprintf(cycleName, "out/cyclictest-%d.dot", i);
		ofstream dotfile(cycleName);
		dotfile << reducedCycle.toGraphviz();
		dotfile.close();
	}
	return true;
}
