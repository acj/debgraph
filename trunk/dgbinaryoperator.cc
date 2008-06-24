#include "dgbinaryoperator.h"

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
	Edge *e;
	for (GraphIterator i = sink.begin(); i != sink.end(); ++i) {
		string fromNodeId, toNodeId;
		set<Edge*> outEdges;
		set<Edge*>::const_iterator edgeIter;
		// Nodes in source #1
		fromNodeId = (*i)->getId();
		if (source1.hasNode(fromNodeId)) {
			n = source1.findNode(fromNodeId);
			outEdges = n->getOutEdges();
			for (edgeIter = outEdges.begin(); edgeIter != outEdges.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (sink.hasNode(toNodeId)) {
					e = Edge::createEdge(*i, sink.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
		// Nodes in source #2
		if (source2.hasNode((*i)->getId())) {
			n = source2.findNode((*i)->getId());
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
