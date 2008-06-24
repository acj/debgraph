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
	for (size_t i = 0; i < cycles.size(); ++i) {
		// Write out the original
		//sprintf(cycleName, "out/cyclictest-orig-%d.dot", i);
		//ofstream dotfile_orig(cycleName);
		//dotfile_orig << cycles[i].toGraphviz();
		//dotfile_orig.close();
		// Write out the reduced (BINARYNAME only) version
		fc.mergePackageVersions(cycles[i]);
		sprintf(cycleName, "out/cyclictest-%d.dot", i);
		ofstream dotfile(cycleName);
		dotfile << cycles[i].toGraphviz();
		dotfile.close();
	}
	return true;
}
