#include <iostream>
#include <fstream>
#include "graph_types.h"
#include "finddeps.h"
#include "finddepstest.h"

bool FindDepsTest::run(Graph &g) {
	// FIXME: Need a more robust (and self-contained) test case
	string target = "Binary:libc6:2.7-12:i386";
	Node *n = g.findNode(target);
	if (n == 0) {
		cerr << "Could not find node: " << target << endl;
		return false;
	}
	Graph result = FindDeps(g, n).execute();
	ofstream dotfile("out/finddeps.dot");
	dotfile << result.toGraphviz();
	dotfile.close();
	return true;
}
