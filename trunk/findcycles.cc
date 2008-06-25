#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include "findcycles.h"

// NB: Constructor defined in header file.

FindCycles::~FindCycles() {
	// XXX Deallocate memory for traversal data
}

bool NodeNameComparator(const Node *a, const Node *b) {
	return (const_cast<Node*>(a))->getProperty("Package") < 
			(const_cast<Node*>(b))->getProperty("Package");
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

string sourcePackage(Node *p) {
	set<Edge*> e = p->getOutEdges();
	set<Edge*>::iterator ei;
	for (ei = e.begin(); ei != e.end(); ei++) {
		Node *on = const_cast<Node*>((*ei)->getToNode());
		if (on->getType() == Entity::SOURCE) {
			return on->getProperty("Package");
		}
	}
	return "";
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
	set<Edge*> s = n->getOutEdges();
	set<Edge*>::const_iterator i;
	for (i = s.begin(); i != s.end(); i++) {
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
			Graph newCycle;
			for (list<Node*>::const_iterator i = tl.begin(); i != tl.end(); i++) {
					newCycle.addNode(new Node(**i));
			}
			copyConsistentEdges(operand, newCycle);
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
	set<Edge*> s = n->getOutEdges();
	set<Edge*>::iterator i;
	for (i = s.begin(); i != s.end(); i++) {
		Node *on = const_cast<Node*>((*i)->getToNode());
		markRecursive(on, depth, mark);
	}
}

vector<Graph>& FindCycles::getCycles() {
	return cycles;
}

Graph& FindCycles::execute() {
	Node *startNode = operand.findNode(startNodeId);
	if (startNode == NULL) {
		cout << "Error: Starting node \"" << startNodeId 
			 << "\" not found in graph" << endl;
	}
	else {
		// Mark as unhandled in our release.
		markRecursive(startNode, 4, 0);
		int N = 0;
		list<Node*> l;
		tarjan(operand, startNode, &l, &N);
	}
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
