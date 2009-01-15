#include <fstream>
#include <iostream>
#include "graph_types.h"
#include "graph.h"
#include "graphtest.h"

bool GraphTest::run(Graph &g) {
	Graph addDeleteGraph;
	Node *releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = addDeleteGraph.addNode(releaseNode, Graph::DISCARD_DUP);

	if ( !(addDeleteGraph.size() == 1
		&& addDeleteGraph.hasNode("Release:semistable")) ) {
		cout << "Add/delete Node failed" << endl;
		return false;
	}
	addDeleteGraph.deleteNode("Release:semistable");
	if ( !(addDeleteGraph.size() == 0
		&& !addDeleteGraph.hasNode("Release:semistable")) ) {
		return false;
	}

	// Test the copy constructor
	Graph g1;

	releaseNode = new Node("Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1.addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1.addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1.addNode(componentNode, Graph::DISCARD_DUP);
	g1.createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	g1.createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);
	ofstream dotfile;
	dotfile.open("out/graphtest-orig.dot");
	dotfile << g1.toGraphviz();
	dotfile.close();

	if ( !(g1.hasEdge(releaseNode, componentNode, Entity::CONTAINS)
			&& g1.hasEdge(componentNameNode, componentNode, Entity::HAS_INSTANCE)) ) {
		cout << "\n\tMissing at least one edge" << endl;
		return false;
	}
	Graph g1_copy(g1);
	if ( !(g1_copy.hasNode("Release:semistable")
		&& g1_copy.findNode("Release:semistable") != NULL
		&& g1_copy.hasNode("ComponentName:main")
		&& g1_copy.findNode("ComponentName:main") != NULL
		&& g1_copy.hasNode("Component:semistable:main")
		&& g1_copy.findNode("Component:semistable:main") != NULL
		&& g1_copy.size() == 3) ) {
		cout << "\n\tFailed to copy Graph object." << endl;
		return false;
	}
	dotfile.open("out/graphtest-copy.dot");
	dotfile << g1_copy.toGraphviz();
	dotfile.close();
	return true;
}
