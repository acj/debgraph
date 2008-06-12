#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "findreversedeps.h"
#include "findreversedepstest.h"

bool FindReverseDepsTest::run(Graph &g) {
	// FIXME: Need a more robust (and self-contained) test case
	string target = "Binary:arping:2.05-2:i386";
	Node *n = g.findNode(target);
	if (n == 0) {
		cerr << "Could not find node: " << target << endl;
		return false;
	}
	Graph result = FindReverseDeps(g, n).execute();
	ofstream dotfile("out/findreversedeps.dot");
	dotfile << result.toGraphviz();
	dotfile.close();
	return true;
}
