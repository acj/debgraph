#include <fstream>
#include <iostream>
#include "graph.h"
#include "graph_types.h"
#include "filter.h"
#include "filtertest.h"

bool FilterTest::run(Graph &g) {
	Graph g1;
	Edge *e;
	// g1
	Node *releaseNode = new Node("Release:semistable");
	releaseNode->addProperty("id", "Release:semistable");
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g1.addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:main");
	componentNameNode->addProperty("id", "ComponentName:main");
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g1.addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:semistable:main");
	componentNode->addProperty("id", "Component:semistable:main");
	componentNode->setType(Entity::COMPONENTNAME);
	componentNode = g1.addNode(componentNode, Graph::DISCARD_DUP);
	e = Edge::createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = Edge::createEdge(componentNameNode, componentNode,
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);

	FilterProperties fp;
	FilterPair fpair_id = { string("id"), string("Component:semistable:main") };
	fp.push_back(fpair_id);
	Filter f(g1, fp, FILTER_AND);
	Graph result = f.execute();

	if ( !(result.size() == 1
		&& result.hasNode("Component:semistable:main")
		&& !result.hasNode("ComponentName:main")
		&& !result.hasNode("Release:semistable")) ) {
		return false;
	}

	// Run a test on the full Debian graph
	FilterProperties fProperties;
	FilterPair fPairSection = { string("Section"), string("games") };
	FilterPair fPairArch = { string("Architecture"), string("arm") };
	fProperties.push_back(fPairSection);
	fProperties.push_back(fPairArch);
	Filter fDeb(g, fProperties, FILTER_AND);
	Graph resultDeb = fDeb.execute();
	cout << "\tNodes matching Section:games AND Architecture:arm "
		 << resultDeb.size() << endl;

	// Refine the search again	
	FilterProperties fPropertiesPriority;
	FilterPair fPairPriority = { string("Priority"), string("extra") };
	Filter fDebPri(resultDeb, fPropertiesPriority, FILTER_AND);
	fDebPri.addCriterion(fPairPriority);
	Graph resultPriority = fDebPri.execute();
	cout << "\tNodes matching Section:games AND Architecture:arm AND Priority:extra "
		 << resultPriority.size() << endl;

	ofstream dotfile;
	dotfile.open("out/filtertest.dot");
	dotfile << resultPriority.toGraphviz();
	dotfile.close();
		
	return true;
}
