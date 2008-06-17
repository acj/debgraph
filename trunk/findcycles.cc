#include <iostream>
#include <set>
#include <stack>
#include <vector>
#include "entity.h"
#include "findcycles.h"

// NB: Constructor defined in header file.

FindCycles::~FindCycles() {
}

Graph& FindCycles::execute() {
	// Find a node with at least one in- and out-edge to seed the DFS
	vector<Node*> traceVector;
	set<string> traceNodeNames;
	set<string> visitedNodeNames;
	stack< pair<Node*,Node*> > nodesToVisit; // to-visit node, sponsoring node
	for (GraphIterator i = operand.begin(); i != operand.end(); ++i) {
		if (visitedNodeNames.find((*i)->getId()) == visitedNodeNames.end()
				&& (*i)->getOutEdges().size() > 0 
				&& (*i)->getInEdges().size() > 0) {
			nodesToVisit.push(pair<Node*,Node*>(*i, *i));
			Node *curNode;
			while (!nodesToVisit.empty()) {
				curNode = nodesToVisit.top().first;
				nodesToVisit.pop();
				visitedNodeNames.insert(curNode->getId());
				traceNodeNames.insert(curNode->getId());
				traceVector.push_back(curNode);
				for (EdgeSetConstIterator edgeIter = curNode->getOutEdges().begin();
						edgeIter != curNode->getOutEdges().end(); ++edgeIter) {
					Entity::EntityType eType = (*edgeIter)->getType();
					if (eType != Entity::DEPENDS 
							&& eType != Entity::PRE_DEPENDS 
							&& eType != Entity::HAS_INSTANCE 
							&& eType != Entity::HAS_VERSION) {
						continue;
					}
					if (traceNodeNames.find((*edgeIter)->getToNode()->getId())
							!= traceNodeNames.end()) {
						// Cycle!  Beginning with the node most recently
						// visited, follow the cycle backwards until we reach
						// the node across the edge (the "to" node).
						string cycleNodeId = (*edgeIter)->getToNode()->getId();
						vector<Node*>::const_reverse_iterator i = traceVector.rbegin();
						Graph newCycleGraph;
						Node *startNode = new Node(**i);
						Node *previousNode = startNode;
						Node *nextNode;
						newCycleGraph.addNode(startNode, Graph::FAIL_DUP);
						++i; // Move past the first item
						for (; i != traceVector.rend(); ++i) {
							if ((*i)->getId() == cycleNodeId) {
								// Done copying the cycle.  Add last node/edge
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
						if (cycles.size() == cycle_limit) {
							goto finish;
						}
					}
					else {
						pair<Node*,Node*> nodePair((Node *)(*edgeIter)->getToNode(),
								curNode);
						nodesToVisit.push(nodePair);
					}
				}
				// Prune exhausted branches from the trace vector
				while (!nodesToVisit.empty()
						&& traceVector.back() != nodesToVisit.top().second) {
					traceNodeNames.erase(traceVector.back()->getId());
					traceVector.pop_back();
				}
			}
		}
		else {
			visitedNodeNames.insert((*i)->getId());
		}
	}
finish:
	// FIXME: Do something useful for the result
	return result;
}

vector<Graph>& FindCycles::getCycles() {
	return cycles;
}
