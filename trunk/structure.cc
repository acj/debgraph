#include <iostream>
#include <fstream>
#include <set>

#include "structure.h"
#include "entity.h"

string Structure::getName() {
	return "Structure";
}

void Structure::run(Graph &g) {
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
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
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
			sDeps.insert(dep);
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
}
