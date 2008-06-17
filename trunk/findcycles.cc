#include <iostream>
#include <set>
#include <vector>
#include "entity.h"
#include "findcycles.h"

// NB: Constructor defined in header file.

FindCycles::~FindCycles() {
}

Graph& FindCycles::execute() {
	// Find a node with at least one in- and out-edge to seed the DFS
	vector<Node*> depVector;
	for (GraphIterator i = operand.begin(); i != operand.end(); ++i) {
		if ((*i)->getOutEdges().size() > 0 && (*i)->getInEdges().size() > 0) {
			findCycles(*i);
			break;
		}
	}
	// FIXME: Do something useful for the result
	//addConsistentEdges();
	return result;
}

vector<Graph>& FindCycles::getCycles() {
	return cycles;
}

void FindCycles::findCycles(Node *curNode) {
	cout << "Visiting node " << curNode->getId() << endl;
	visitedNodes.insert(curNode->getId());
	depVector.push_back(curNode);
	for (EdgeSetConstIterator edgeIter = curNode->getOutEdges().begin();
			edgeIter != curNode->getOutEdges().end(); ++edgeIter) {
		Entity::EntityType eType = (*edgeIter)->getType();
		if (eType != Entity::DEPENDS 
				&& eType != Entity::PRE_DEPENDS 
				&& eType != Entity::HAS_INSTANCE 
				&& eType != Entity::HAS_VERSION) {
			continue;
		}
		if (visitedNodes.find((*edgeIter)->getToNode()->getId()) != visitedNodes.end()) {
			// FIXME This does not properly preserve the edge types
			//cout << "CYCLE: " << curNode->getId() << " <-> " << (*edgeIter)->getToNode()->getId() << endl; 
			
			// Cycle!  Beginning with the node most recently
			// visited, follow the cycle backwards until we reach
			// the node across the edge (the "to" node).
			string cycleNodeId = (*edgeIter)->getToNode()->getId();
			vector<Node*>::const_reverse_iterator i = depVector.rbegin();
			Graph newCycleGraph;
			Node *startNode = new Node(**i);
			Node *previousNode = startNode;
			Node *nextNode;
			newCycleGraph.addNode(startNode, Graph::FAIL_DUP);
			++i; // Move past the first item
			//cout << "Start node: " << startNode->getId() << endl;
			for (; i != depVector.rend(); ++i) {
				//cout << "Traversing node: " << (*i)->getId() << endl;
				if ((*i)->getId() == cycleNodeId) {
					// Done copying the cycle.  Add the last node/edge.
					Node *endNode = new Node(**i);
					newCycleGraph.addNode(endNode, Graph::FAIL_DUP);
					Edge::createEdge(endNode, previousNode,
							(*edgeIter)->getType(), Edge::IGNORE_DUP);
					Edge::createEdge(startNode, endNode,
							(*edgeIter)->getType(), Edge::IGNORE_DUP);
					break;
				}
				else {
					nextNode = new Node(**i);
					newCycleGraph.addNode(nextNode, Graph::FAIL_DUP);
					Edge::createEdge(nextNode, previousNode, 
								(*edgeIter)->getType(), Edge::IGNORE_DUP);	
					previousNode = nextNode;
				}
			}
			cycles.push_back(newCycleGraph);
		}
		else {
			findCycles((Node *)(*edgeIter)->getToNode());
		}
	}
	depVector.pop_back();
	visitedNodes.erase(curNode->getId());
	//cout << "Leaving node " << curNode->getId() << endl;
}
