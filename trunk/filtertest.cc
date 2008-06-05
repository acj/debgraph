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
	FilterRule fpair_id = { string("id"), EQUALS, string("Component:semistable:main") };
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
	FilterRule fRuleSection = { string("Section"), EQUALS, string("games") };
	FilterRule fRuleArch = { string("Architecture"), EQUALS, string("arm") };
	fProperties.push_back(fRuleSection);
	fProperties.push_back(fRuleArch);
	Filter fDeb(g, fProperties, FILTER_AND);
	Graph resultDeb = fDeb.execute();

	// Refine the search to include to include Priority EQUALS extra	
	FilterProperties fPropertiesEQUALS;
	FilterRule fRuleEQUALS = { string("Priority"), EQUALS, string("extra") };
	Filter fDebPri(resultDeb, fPropertiesEQUALS, FILTER_AND);
	fDebPri.addCriterion(fRuleEQUALS);
	Graph resultEQUALS = fDebPri.execute();
		
	// Refine the search again to include Priority NEQUALS extra
	FilterProperties fPropertiesNEQUALS;
	FilterRule fRuleNEQUALS = { string("Priority"), NEQUALS, string("extra") };
	Filter fDebNOTPri(resultDeb, fPropertiesNEQUALS, FILTER_AND);
	fDebNOTPri.addCriterion(fRuleNEQUALS);
	Graph resultNEQUALS = fDebNOTPri.execute();
	if (resultDeb.size() != (resultEQUALS.size() + resultNEQUALS.size())) {
		cout << "EQUALS: Total size mismatch between complementary Node sets" << endl;
		cout << "\t" << resultDeb.size() << " != " << resultEQUALS.size()
			 << " + " << resultNEQUALS.size() << endl;
		return false;
	}

	// Refine the search again to include Package CONTAINS a7xpg
	FilterProperties fPropertiesCONTAINS;
	FilterRule fRuleCONTAINS = { string("Package"), CONTAINS, string("chess") };
	Filter fDebCONTAINS(resultDeb, fPropertiesCONTAINS, FILTER_AND);
	fDebCONTAINS.addCriterion(fRuleCONTAINS);
	Graph resultCONTAINS = fDebCONTAINS.execute();

	// Refine the search again to Package NCONTAINS a7xpg.  Should be
	// complementary to the previous result.
	FilterProperties fPropertiesNCONTAINS;
	FilterRule fRuleNCONTAINS = { string("Package"), NCONTAINS, string("chess") };
	Filter fDebNCONTAINS(resultDeb, fPropertiesNCONTAINS, FILTER_AND);
	fDebNCONTAINS.addCriterion(fRuleNCONTAINS);
	Graph resultNCONTAINS = fDebNCONTAINS.execute();
	if (resultDeb.size() != (resultCONTAINS.size() + resultNCONTAINS.size())) {
		cout << "\tNCONTAINS: "
			 << "Total size mismatch between complementary Node sets"
			 << "\n\t" << resultDeb.size() << " != " << resultCONTAINS.size()
			 << " + " << resultNCONTAINS.size() << endl;
		return false;
	}

	FilterProperties fPropertiesDepends;
	Filter fDependencies(g, fPropertiesDepends, FILTER_OR);
	FilterRule fRuleDepends1 = { string("Package"), EQUALS, string("3dchess") };
	FilterRule fRuleDepends2 = { string("Package"), EQUALS, string("libc6") };
	fDependencies.addCriterion(fRuleDepends1);
	fDependencies.addCriterion(fRuleDepends2);
	Graph dependsResult = fDependencies.execute();

	ofstream dotfile("out/filtertest-edges.dot");
	dotfile << dependsResult.toGraphviz();
	dotfile.close();

	return true;
}
