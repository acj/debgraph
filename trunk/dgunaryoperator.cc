#include "dgunaryoperator.h"

DGUnaryOperator::DGUnaryOperator(Graph &g1) 
	: operand(g1) {
}

DGUnaryOperator::~DGUnaryOperator() {
}

void DGUnaryOperator::copyConsistentEdges(Graph &source, Graph &sink) {
	for (GraphIterator i = sink.begin(); i != sink.end(); ++i) {
		Node *n;
		Edge *e;
		string fromNodeId, toNodeId;
		set<Edge*> outEdges;
		set<Edge*>::const_iterator edgeIter;
		fromNodeId = (*i)->getId();
		if (source.hasNode(fromNodeId)) {
			n = source.findNode(fromNodeId);
			outEdges = n->getOutEdges();
			for (edgeIter = outEdges.begin(); edgeIter != outEdges.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (sink.hasNode(toNodeId)) {
					e = Edge::createEdge(*i, sink.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
	}
}
