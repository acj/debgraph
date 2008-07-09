#include "dgunaryoperator.h"

DGUnaryOperator::DGUnaryOperator(Graph &g1) 
	: operand(g1) {
}

DGUnaryOperator::~DGUnaryOperator() {
}

void DGUnaryOperator::copyConsistentEdges(Graph &source, Graph &sink) {
	Node *n;
	for (GraphIterator i = sink.begin(); i != sink.end(); ++i) {
		string fromNodeId, toNodeId;
		EdgeSetIterator edgeIter;
		fromNodeId = (*i)->getId();
		if (source.hasNode(fromNodeId)) {
			n = source.findNode(fromNodeId);
			EdgeSet &outEdges = source.getOutEdges(n);
			for (edgeIter = outEdges.begin(); edgeIter != outEdges.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (sink.hasNode(toNodeId)) {
					sink.createEdge(*i, sink.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
	}
}
