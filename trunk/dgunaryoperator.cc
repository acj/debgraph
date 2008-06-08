#include "dgunaryoperator.h"

DGUnaryOperator::DGUnaryOperator(Graph &g1) 
	: operand(g1) {
}

DGUnaryOperator::~DGUnaryOperator() {
}

void DGUnaryOperator::addConsistentEdges() {
	for (GraphIterator i = result.begin(); i != result.end(); ++i) {
		Node *n;
		Edge *e;
		string fromNodeId, toNodeId;
		set<Edge*> outEdges;
		set<Edge*>::const_iterator edgeIter;
		fromNodeId = (*i)->getId();
		if (operand.hasNode(fromNodeId)) {
			n = operand.findNode(fromNodeId);
			outEdges = n->getOutEdges();
			for (edgeIter = outEdges.begin(); edgeIter != outEdges.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (result.hasNode(toNodeId)) {
					e = Edge::createEdge(*i, result.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
	}
}
