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
		GraphIndexT& getIndex();
	public:
		friend class GraphIterator;
		Graph();
		~Graph();
		Graph(const Graph &);
		Graph& operator=(const Graph &);
		void clone(const Graph &);
		Node* addNode(Node *node, AddFlag flag = FAIL_DUP);
		void deleteNode(const string &);
		Node* findNode(const string &) const;
		bool hasNode(const string &) const;
		GraphIterator& begin() const;
		GraphIterator& end() const;
		int size() const;
		string toGraphviz();
};

#endif /* GRAPH_H */
