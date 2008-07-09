#include "dgbinaryoperator.h"
#include "graph.h"

DGBinaryOperator::DGBinaryOperator(Graph &g1, Graph &g2) 
	: l_operand(g1), r_operand(g2) {
}

DGBinaryOperator::~DGBinaryOperator() {
}

Graph& DGBinaryOperator::execute() {
	result = Graph();
	return result;
}

void DGBinaryOperator::copyConsistentEdges(Graph &source1, Graph &source2,
		Graph &sink) {
	Node *n;
	for (GraphIterator i = sink.begin(); i != sink.end(); ++i) {
		string fromNodeId, toNodeId;
		EdgeSetIterator edgeIter;
		// Nodes in source #1
		fromNodeId = (*i)->getId();
		if (source1.hasNode(fromNodeId)) {
			n = source1.findNode(fromNodeId);
			EdgeSet &outEdgesS1 = source1.getOutEdges(n);
			for (edgeIter = outEdgesS1.begin(); edgeIter != outEdgesS1.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (sink.hasNode(toNodeId)) {
					sink.createEdge(*i, sink.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
		// Nodes in source #2
		if (source2.hasNode((*i)->getId())) {
			n = source2.findNode((*i)->getId());
			EdgeSet &outEdgesS2 = source2.getOutEdges(n);
			for (edgeIter = outEdgesS2.begin(); edgeIter != outEdgesS2.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (sink.hasNode(toNodeId)) {
					sink.createEdge(*i, sink.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
	}
}
