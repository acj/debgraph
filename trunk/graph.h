#ifndef GRAPH_H
#define GRAPH_H

#include <map>

#include "node.h"

using namespace std;

class Graph {
	public:
		enum AddFlag {FAIL_DUP,
				IGNORE_DUP,
				DISCARD_DUP};
	protected:
		/* would be cool if this could be a hash_multimap */
		multimap<string, Node*> index;
	public:
		Graph();
		~Graph();
		Node *addNode(Node *node, AddFlag flag = FAIL_DUP);
		Node* findNode(const string &id);
		bool hasNode(const string &id);
		multimap<string, Node*>& getIndex();
		int size();
};

#endif /* GRAPH_H */
