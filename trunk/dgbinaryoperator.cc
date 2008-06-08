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

void DGBinaryOperator::addConsistentEdges() {
	for (GraphIterator i = result.begin(); i != result.end(); ++i) {
		Node *n;
		Edge *e;
		string fromNodeId, toNodeId;
		set<Edge*> outEdges;
		set<Edge*>::const_iterator edgeIter;
		// Nodes in left Graph
		fromNodeId = (*i)->getId();
		if (l_operand.hasNode(fromNodeId)) {
			n = l_operand.findNode(fromNodeId);
			outEdges = n->getOutEdges();
			for (edgeIter = outEdges.begin(); edgeIter != outEdges.end(); ++edgeIter) {
				toNodeId = (*edgeIter)->getToNode()->getId();
				if (result.hasNode(toNodeId)) {
					e = Edge::createEdge(*i, result.findNode(toNodeId), (*edgeIter)->getType(), Edge::IGNORE_DUP);
				}
			}
		}
		// Nodes in right Graph
		if (r_operand.hasNode((*i)->getId())) {
			n = r_operand.findNode((*i)->getId());
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
