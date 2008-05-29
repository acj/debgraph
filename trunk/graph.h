#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include "node.h"
#include "graph.h"
#include "graphiterator.h"
#include "graph_types.h"

using namespace std;

class GraphIterator;

class Graph {
	public:
		enum AddFlag {FAIL_DUP,
				IGNORE_DUP,
				DISCARD_DUP};
	protected:
		GraphIndexT index;
	public:
		friend class GraphIterator;
		Graph();
		~Graph();
		Graph(Graph&);
		Graph& operator=(Graph&);
		Node* addNode(Node *node, AddFlag flag = FAIL_DUP);
		Node* findNode(const string &id);
		bool hasNode(const string &id);
		GraphIndexT &getIndex();
		GraphIterator &begin();
		GraphIterator &end();
		int size();
};

#endif /* GRAPH_H */
