#include <iostream>
#include <fstream>
#include <set>

#include "structure.h"
#include "entity.h"

string Structure::getName() {
	return "Structure";
}

bool Structure::run(Graph &g) {
	ofstream ofile;
	ofile.open("out/structure.html");
	
	ofstream dotfile;
	dotfile.open("out/structure.dot");

	ofile << "<h1>Graph Template Structure</h1>" << endl;
	dotfile << "digraph \"Graph Template\" {" << endl;
	dotfile << "nodesep=0.1" << endl;

	set<string> archs;
	set<string> comps;
	set<string> releases;
	
	multimap<string, Node*> nIndex = g.getIndex();
	set<string> sDeps;
	// insert standard edges that replace some others 
	sDeps.insert("BINARY -> BINARYNAME [ label = \"(pre)dep, recommends, suggests\" ]");
	sDeps.insert("BINARY -> OR [ label = \"(pre)dep, recommends, suggests\" ]");
	sDeps.insert("BINARY -> VIRTUAL [ label = \"(pre)dep, recommends, suggests\" ]");
	sDeps.insert("OR -> VIRTUAL [ label = \"(pre)dep, recommends, suggests\" ]");
	sDeps.insert("OR -> BINARYNAME [ label = \"(pre)dep, recommends, suggests\" ]");
	// which ones to remove from graph
	set<string> blacklist;
	blacklist.insert("BINARY -> BINARYNAME [ label = \"DEPENDS\" ]");
	blacklist.insert("BINARY -> BINARYNAME [ label = \"PRE_DEPENDS\" ]");
	blacklist.insert("BINARY -> BINARYNAME [ label = \"RECOMMENDS\" ]");
	blacklist.insert("BINARY -> BINARYNAME [ label = \"SUGGESTS\" ]");
	blacklist.insert("BINARY -> OR [ label = \"DEPENDS\" ]");
	blacklist.insert("BINARY -> OR [ label = \"PRE_DEPENDS\" ]");
	blacklist.insert("BINARY -> OR [ label = \"RECOMMENDS\" ]");
	blacklist.insert("BINARY -> OR [ label = \"SUGGESTS\" ]");
	blacklist.insert("BINARY -> VIRTUAL [ label = \"DEPENDS\" ]");
	blacklist.insert("BINARY -> VIRTUAL [ label = \"PRE_DEPENDS\" ]");
	blacklist.insert("BINARY -> VIRTUAL [ label = \"RECOMMENDS\" ]");
	blacklist.insert("BINARY -> VIRTUAL [ label = \"SUGGESTS\" ]");
	blacklist.insert("OR -> VIRTUAL [ label = \"DEPENDS\" ]");
	blacklist.insert("OR -> VIRTUAL [ label = \"PRE_DEPENDS\" ]");
	blacklist.insert("OR -> VIRTUAL [ label = \"RECOMMENDS\" ]");
	blacklist.insert("OR -> VIRTUAL [ label = \"SUGGESTS\" ]");
	blacklist.insert("OR -> BINARYNAME [ label = \"DEPENDS\" ]");
	blacklist.insert("OR -> BINARYNAME [ label = \"PRE_DEPENDS\" ]");
	blacklist.insert("OR -> BINARYNAME [ label = \"RECOMMENDS\" ]");
	blacklist.insert("OR -> BINARYNAME [ label = \"SUGGESTS\" ]");

	Node *n;
	for (GraphIterator i = g.begin(); i != g.end(); ++i) {
		n = *i;
		if (n->getType() == Entity::COMPONENTNAME) {
			comps.insert(n->getId());
		}
		else if (n->getType() == Entity::ARCHITECTURE) {
			archs.insert(n->getId());
		}
		else if (n->getType() == Entity::RELEASE) {
			releases.insert(n->getId());
		}
		string fromNodeType = n->getTypeString();
		set<Edge*> edges = n->getOutEdges();
		for (set<Edge*>::iterator j = edges.begin(); j != edges.end(); j++) {
			Edge *cedge = *j;
			string edgeType = cedge->getTypeString();
			Node *cNode = const_cast<Node*>(cedge->getToNode());
			string toNodeType = cNode->getTypeString();
			string dep = fromNodeType + " -> " + toNodeType + 
				" [ label = \"" + edgeType + "\" ]";
			if (blacklist.find(dep) == blacklist.end()) {
				sDeps.insert(dep);
			}
		}
	}
	for (set<string>::iterator i = sDeps.begin(); i != sDeps.end(); i++) {
		dotfile << "\t" << *i << endl;
	}
	dotfile << "}" << endl;
	dotfile.close();

	ofile << "<img src=\"structure.png\">" << endl;

	ofile << "<br/><b>Nodes of type COMPONENTNAME</b><br/>" << endl;
	for (set<string>::iterator i = comps.begin(); i != comps.end(); i++) {
		ofile << *i << "<br/>" << endl;
	}
	ofile << "<br/><b>Nodes of type ARCHITECTURE</b><br/>" << endl;
	for (set<string>::iterator i = archs.begin(); i != archs.end(); i++) {
		ofile << *i << "<br/>" << endl;
	}
	ofile << "<br/><b>Nodes of type RELEASE</b><br/>" << endl;
	for (set<string>::iterator i = releases.begin(); i != releases.end(); i++) {
		ofile << *i << "<br/>" << endl;
	}

	ofile.close();
	addItem("structure.html", "", "Graph Template Structure");
	return true;
}
