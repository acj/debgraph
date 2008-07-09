#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "findcycles.h"

// NB: Constructor defined in header file.

FindCycles::~FindCycles() {
	// XXX Deallocate memory for traversal data
}

static inline int min(int a, int b) {
	return a < b ? a : b;
}

string hex(void *p) {
	char buf[67];
	string s;
	sprintf(buf, "X%08X", (unsigned int)p);
	s = buf;
	return s;
}

// Implementation of Tarjan's algorithm for finding strongly connected
// components.
// 
// Arguments:
// 	 g: the graph to be traversed
// 	 n:	an arbitrary node to seed the DFS
// 	 l:	trace of nodes in the current DFS tree	
// 	 N: Tarjan's index (indicates discovery order)
// 	 dist: the distribution that we wish to examine for cycles (e.g. "stable")
int FindCycles::tarjan(Graph &g, Node *n, list<Node*> *l, int *N) {
	l->push_back(n);
	NodeState *nState = traversalData[n->getId()];
	nState->dfs = *N;
	nState->low = *N;
	(*N)++;
	int retval = 0;
	string nodePackName = n->getProperty("Package");
	/* recursion */
	NodeState *onState;
	EdgeSet &edgeSet = g.getOutEdges(n);
	EdgeSetIterator i;
	for (i = edgeSet.begin(); i != edgeSet.end(); i++) {
		/* examine this link */
		if (allowedEntities.find((*i)->getType()) != allowedEntities.end()) {
			Node *on = const_cast<Node*>((*i)->getToNode());
			onState = traversalData[on->getId()];
			if (onState->mark == 0) {
				onState->mark = 1; // mark as discovered
				retval += tarjan(g, on, l, N);
				nState->low = min(nState->low, onState->low);
			}
			else if (onState->mark == 1) {
				nState->low = min(nState->low, onState->dfs);
			}
		}
	}
	if (nState->low == nState->dfs) {
		/* found an SCC - unwind the stack to discover its nodes */
		Node *cn;
		list<Node*> tl;
		do {
			cn = l->back();
			l->pop_back();
			NodeState *cnState = traversalData[cn->getId()];
			cnState->mark = 2; // mark as handled
			tl.push_back(cn);
		} while (cn != n);
		if (tl.size() > 1) {
			Graph *newCycle = new Graph();
			registerObject(newCycle);
			for (list<Node*>::const_iterator i = tl.begin(); i != tl.end(); i++) {
					newCycle->addNode(*i);
			}
			copyConsistentEdges(operand, *newCycle);
			cycles.push_back(newCycle);
		}
	}
	return retval;
}

void FindCycles::markRecursive(Node *n, int depth, int mark) {
	if (depth == 0) {
		return;
	}
	if (n->getType() != Entity::OR) {
		depth--;
	}
	NodeState *nState = traversalData[n->getId()];
	nState->mark = mark;
	EdgeSet &s = operand.getOutEdges(n);
	EdgeSetIterator i;
	for (i = s.begin(); i != s.end(); i++) {
		Node *on = const_cast<Node*>((*i)->getToNode());
		markRecursive(on, depth, mark);
	}
}

vector<Graph*>& FindCycles::getCycles() {
	return cycles;
}

Graph& FindCycles::execute() {
	if (!operand.hasNode(this->startNodeId)) {
		cout << "\n\tError: Starting node \"" << this->startNodeId 
			 << "\" not found in graph" << endl;
	}
	else {
		Node *startNode = operand.findNode(this->startNodeId);
		// Mark as unhandled in our release.
		markRecursive(startNode, 4, 0);
		int N = 0;
		list<Node*> l;
		tarjan(operand, startNode, &l, &N);
		if (discoverAllNodes) {
			// Re-run the algorithm for any undiscovered nodes
			NodeState *nState;
			for (GraphIterator gi = operand.begin(); gi != operand.end(); ++gi) {
				nState = traversalData[(*gi)->getId()];
				if (nState->mark == 0) {
					int N = 0;
					list<Node*> l;
					tarjan(operand, *gi, &l, &N);
				}
			}
		}
	}
	// XXX Do something useful for output...
	return operand;
}

void FindCycles::initTraversalData() {
	for (GraphIterator gi = operand.begin(); gi != operand.end(); ++gi) {
		Node *n = *gi;
		NodeState *nState = new NodeState; // dfs, low, mark
		nState->dfs = 0;
		nState->low = 0;
		nState->mark = 0;
		if (n->getType() == Entity::BINARY) {
			/* mark all binaries as handled */
			nState->mark = 2;
		}
		traversalData.insert(pair<string,NodeState*>(n->getId(), nState));
	}
}
