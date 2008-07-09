#include "dgoperator.h"

DGOperator::DGOperator() {
}

DGOperator::~DGOperator() {
}

void DGOperator::mergePackageVersions(Graph &g) {
	Node *nameNode, *verNode, *otherNode;
	Edge *copyEdge;
	EdgeSetIterator nameEdgeIter, verEdgeIter;
	for (GraphIterator i = g.begin(); i != g.end(); ++i) {
		if ((*i)->getType() == Entity::BINARYNAME) {
			nameNode = *i;
			EdgeSet &nameOutEdges = g.getOutEdges(*i);
			for (nameEdgeIter = nameOutEdges.begin(); 
					nameEdgeIter != nameOutEdges.end(); 
					++nameEdgeIter) {
				if ((*nameEdgeIter)->getType() == Entity::HAS_VERSION) {
					copyEdge = *nameEdgeIter;
					verNode = const_cast<Node*>(copyEdge->getToNode());
					EdgeSet &verOutEdges = g.getOutEdges(verNode);
					for (verEdgeIter = verOutEdges.begin();
							verEdgeIter != verOutEdges.end();
							++verEdgeIter) {
						otherNode = const_cast<Node*>((*verEdgeIter)->getToNode());
						g.createEdge(nameNode, otherNode, 
								(*verEdgeIter)->getType(), Edge::IGNORE_DUP);
					}
					// XXX Do we need to consider inbound edges?
					g.deleteNode(verNode->getId());
				}
			}
		}
	}
}

void DGOperator::registerObject(void *p) {
	MemAcct *ma = MemAcct::instance();
	ma->addReference(p);
}
