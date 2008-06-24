#include "dgoperator.h"

DGOperator::DGOperator() {
}

DGOperator::~DGOperator() {
}

void DGOperator::mergePackageVersions(Graph &g) {
	Node *nameNode, *verNode, *otherNode;
	Edge *copyEdge;
	set<Edge*> nameOutEdges, verOutEdges;
	set<Edge*>::const_iterator nameEdgeIter, verEdgeIter;
	for (GraphIterator i = g.begin(); i != g.end(); ++i) {
		if ((*i)->getType() == Entity::BINARYNAME) {
			nameNode = *i;
			nameOutEdges = (*i)->getOutEdges();
			for (nameEdgeIter = nameOutEdges.begin(); 
					nameEdgeIter != nameOutEdges.end(); 
					++nameEdgeIter) {
				if ((*nameEdgeIter)->getType() == Entity::HAS_VERSION) {
					copyEdge = *nameEdgeIter;
					verNode = const_cast<Node*>(copyEdge->getToNode());
					for (verEdgeIter = verNode->getOutEdges().begin();
							verEdgeIter != verNode->getOutEdges().end();
							++verEdgeIter) {
						otherNode = const_cast<Node*>((*verEdgeIter)->getToNode());
						Edge::createEdge(nameNode, otherNode, 
								(*verEdgeIter)->getType(), Edge::IGNORE_DUP);
					}
					// XXX Do we need to consider inbound edges?
					g.deleteNode(verNode->getId());
				}
			}
		}
	}
}
