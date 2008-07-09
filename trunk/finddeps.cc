#include <stack>
#include "finddeps.h"

// NB: Constructor defined in header file.

FindDeps::~FindDeps() {
}

Graph& FindDeps::execute() {
	stack<Node*> depStack;
	Node *node;
	if (nodeList.size() > 0) {
		for (set<Node*>::const_iterator nodeIter = nodeList.begin();
				nodeIter != nodeList.end(); ++nodeIter) {
			depStack.push(*nodeIter);
		}
	}
	else {
		depStack.push(singleNode);
	}
	while (!depStack.empty()) {
		node = depStack.top();
		depStack.pop();
		// Break cycles
		if (result.hasNode(node->getId())) {
			continue;
		}
		result.addNode(node, Graph::FAIL_DUP);
		EdgeSet &edgeSet = operand.getOutEdges(node);
		for (EdgeSetIterator edgeIter = edgeSet.begin();
				edgeIter != edgeSet.end(); ++edgeIter) {
			if ((*edgeIter)->getType() == Entity::DEPENDS
					|| (*edgeIter)->getType() == Entity::PRE_DEPENDS
					|| (*edgeIter)->getType() == Entity::HAS_VERSION
					|| (*edgeIter)->getType() == Entity::HAS_INSTANCE
					|| (*edgeIter)->getType() == Entity::OR) {
				depStack.push((Node *)(*edgeIter)->getToNode());
			}
		}
	}
	copyConsistentEdges(operand, result);
	return result;
}
