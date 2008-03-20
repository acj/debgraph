#include <iostream>
#include <sstream>
#include <fstream>

#include "nulltest.h"
	
string NullTest::getName() {
	return "NullTest";
}

void NullTest::run(Graph &g) {
	ofstream ofile;
	ofile.open("out/nulltest.html");
	
	int edgecount = 0;

	multimap<string, Node*> nIndex = g.getIndex();
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
		set<Edge*> edges = n->getOutEdges();
		edgecount += edges.size();
	}
	
	ofile << "<h1>Statistics</h1>" << endl;
	ofile << "Graph contains " << g.size() << " nodes<br>" << endl;
	ofile.precision(3);
	ofile << "and " << edgecount << " edges (" << (edgecount*1.0/g.size()) << " per node in average)<br>" << endl;
	ofile << endl;
	
	char buf[128];
	snprintf(buf, sizeof(buf), "/proc/%d/status", getpid());
	ifstream procf;
	procf.open(buf);
	if (!procf.is_open()) {
		cerr << "proc file not opened\n" << endl;
	}
	else {
		string line;
		while (!procf.eof()) {
			getline(procf, line);
			istringstream tok(line);
			string name, value;
			getline(tok, name, ':');
			getline(tok, value);
			if (name.compare("VmRSS") == 0) {
				size_t startpos = value.find_first_not_of(" \t");
				size_t endpos = value.find_last_not_of(" \t");
				value = value.substr(startpos, endpos-startpos+1);
				cout << "Memory: " << value << endl;
				ofile << "Memory: " << value << endl;
			}
		}
		procf.close();
	}

	ofile.close();
	addItem("nulltest.html", "", "Graph Statistics");
}
