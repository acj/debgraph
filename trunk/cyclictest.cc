#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "cyclictest.h"
#include "filter.h"
#include "findcycles.h"
#include "finddeps.h"

bool CyclicTest::run(Graph &g) {
	ofstream htmlout("out/cyclictest.html");
	htmlout << "<h1>CYCLICTEST</h1>\n";

	findCyclesInDist(g, "stable", htmlout);
	findCyclesInDist(g, "testing", htmlout);
	findCyclesInDist(g, "unstable", htmlout);
	htmlout.close();
	return true;
}

void CyclicTest::findCyclesInDist(Graph &g, string distro, ofstream &out) {
	out << "<h2>Cyclic Depends (" << distro << ")</h2>" << endl;
	FindCycles fc(g, FindCycles::DEPENDS, "Release:" + distro, false);
	fc.execute();
	vector<Graph>& cycles = fc.getCycles();
	cout << "Found " << cycles.size() << " cycles in Release:" << distro
		 << endl;
	for (size_t i = 0; i < cycles.size(); ++i) {
		// Write out the original Graph (includes HAS_VERSION et al.)
		//sprintf(cycleName, "out/cyclictest-orig-%d.dot", i);
		//ofstream dotfile_orig(cycleName);
		//dotfile_orig << cycles[i].toGraphviz();
		//dotfile_orig.close();
		// Write out the reduced (BINARYNAME only) version
		fc.mergePackageVersions(cycles[i]);
		// Use the first node as a representative for this cycle
		GraphIterator gIter = cycles[i].begin();
		string pngFilename = "dot/" + distro + "-" + (*gIter)->getId() + ".png";
		string dotFilename = "out/dot/" + distro + "-" + (*gIter)->getId() + ".dot";
		// Graphviz output
		ofstream dotFile(dotFilename.c_str());
		dotFile << cycles[i].toGraphviz();
		dotFile.close();
		// HTML output
		out << "* <a href=\"" << pngFilename << "\">";
		for (; gIter != cycles[i].end(); ++gIter) {
			out << (*gIter)->getProperty("Package") << " ";
		}
		out << "</a><br/>" << endl;
	}
}
