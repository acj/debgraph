#include <stack>
#include "findreversedeps.h"
#include <iostream> // XXX

// NB: Constructor defined in header file.

FindReverseDeps::~FindReverseDeps() {
}

Graph& FindReverseDeps::execute() {
	stack<Node*> depStack;
	Node *node;
	if (nodeList.size() > 0) {
		for (set<Node*>::const_iterator nodeIter = nodeList.begin();
				nodeIter != nodeList.end(); ++nodeIter) {
			depStack.push(*nodeIter);
		}
	}
	else {
		return result;
	}
	while (!depStack.empty()) {
		node = depStack.top();
		depStack.pop();
		// Break cycles
		if (result.hasNode(node->getId())) {
			continue;
		}
		result.addNode(node, Graph::FAIL_DUP);
		EdgeSet& inEdges = operand.getInEdges(node);
		for (EdgeSetIterator edgeIter = inEdges.begin();
				edgeIter != inEdges.end(); ++edgeIter) {
			if ((*edgeIter)->getType() == Entity::DEPENDS
					|| (*edgeIter)->getType() == Entity::PRE_DEPENDS
					|| (*edgeIter)->getType() == Entity::HAS_VERSION
					|| (*edgeIter)->getType() == Entity::HAS_INSTANCE
					|| (*edgeIter)->getType() == Entity::OR) {
				depStack.push((Node *)(*edgeIter)->getFromNode());
			}
		}
	}
	copyConsistentEdges(operand, result);
	return result;
}
