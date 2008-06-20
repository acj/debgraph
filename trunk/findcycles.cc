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

int printGraph(list<Node*> &l, ostream &os = cout) {
	int pdc = 0;
	os << "digraph {" << endl
		<< "node [shape=rect];" << endl
		<< "edge [arrowhead=onormal];" << endl;
	list<Node*>::iterator i;
	map<Node*, Node*> replace;
	set<Node*> ls;
	for (i = l.begin(); i != l.end(); i++) {
		ls.insert(ls.begin(), *i);
		if ((*i)->getType() == Entity::BINARYNAME) {
			/* find the corresponding binary */
			list<Node*>::iterator j;
			for (j = l.begin(); j != l.end(); j++) {
				if ((*j)->getType() == Entity::BINARY) {
					string a = (*j)->getProperty("Package");
					string b =(*i)->getProperty("Package");
					if (a == b) {
						replace.insert(
						pair<Node*, Node*>(*j, *i));
					}
				}
			}
		}
	}
	for (i = l.begin(); i != l.end(); i++) {
		set<Edge*> e = (*i)->getOutEdges();
		set<Edge*>::iterator ei;
		for (ei = e.begin(); ei != e.end(); ei++) {
			Node *on = const_cast<Node*>((*ei)->getToNode());
			if (ls.find(on) == ls.end()) {
				continue;
			}
			if (((*ei)->getType() != Entity::DEPENDS) &&
				((*ei)->getType() != Entity::PRE_DEPENDS)) {
				continue;
			}
			/* check if we need to replace this */
			if (replace.find(on) != replace.end()) {
				on = replace.find(on)->second;
			}
			string a = (*i)->getProperty("Package");
			if (a == "") {
				a = hex(*i);
				if ((*i)->getType() == Entity::OR) {
					os << a << "[shape=circle,label=\"OR\"];" << endl;
				}
				else {
					os << a << "[shape=circle,label=\"\"];" << endl;
				}
			}
			unsigned int p;
			while ((p = a.find("-")) != string::npos) {
				a.replace(p,1,"_");
			}
			while ((p = a.find(".")) != string::npos) {
				a.replace(p,1,"_");
			}
			while ((p = a.find("+")) != string::npos) {
				a.replace(p,1,"_");
			}
			string b = on->getProperty("Package");
			if (b == "") {
				b = hex(on);
			}
			while ((p = b.find("-")) != string::npos) {
				b.replace(p,1,"_");
			}
			while ((p = b.find(".")) != string::npos) {
				b.replace(p,1,"_");
			}
			while ((p = b.find("+")) != string::npos) {
				b.replace(p,1,"_");
			}
			if ((*ei)->getType() == Entity::PRE_DEPENDS) {
				pdc++;
			}
		/*	os << "\t" << a << " -> " << b << " [label=";
			switch ((*ei)->getType()) {
				case Entity::PRE_DEPENDS:
					os << "Pre-Depends";
					break;
				case Entity::DEPENDS:
					os << "Depends";
					break;
				default:
					os << "\"\"";
			}
			os << "];" << endl;*/
			os << "\t" << a << " -> " << b << ";" << endl;
		}
	}
	os << "}" << endl;
	return pdc;
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
int FindCycles::tarjan(Graph &g, Node *n, 
	list<Node*> *l, int *N, string dist) {
	l->push_back(n);
	NodeState *nState = traversalData[n->getId()];
	nState->dfs = *N;
	nState->low = *N;
	(*N)++;
	int retval = 0;
	/* recursion */
	set<Edge*> s = n->getOutEdges();
	set<Edge*>::iterator i;
	for (i = s.begin(); i != s.end(); i++) {
		/* examine this link */
		if (allowedEntities.find((*i)->getType()) != allowedEntities.end()) {
			Node *on = const_cast<Node*>((*i)->getToNode());
			NodeState *onState = traversalData[on->getId()];
			if (onState->mark == 0) {
				onState->mark = 1; // mark as discovered
				retval += tarjan(g, on, l, N, dist);
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
		Graph newCycle;
		int bc = 0;
		do {
			cn = l->back();
			l->pop_back();
			NodeState *cnState = traversalData[cn->getId()];
			cnState->mark = 2; // mark as handled
			if (cn->getType() == Entity::BINARY) {
				bc++;
			}
			newCycle.addNode(new Node(*cn));
		} while (cn != n);
		copyConsistentEdges(operand, newCycle);
		cycles.push_back(newCycle);
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
	int N = 0;
	list<Node*> l;
	string release = "unstable";

	tarjan(operand, *(operand.begin()), &l, &N, release);
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
