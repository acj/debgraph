#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <map>
#include "edge.h"
#include "node.h"
#include "graph.h"
#include "graphiterator.h"
#include "graph_types.h"

using namespace std;

typedef set<Edge*> EdgeSet;
typedef set<Edge*>::iterator EdgeSetIterator;

class GraphIterator;

class Graph {
	public:
		enum AddFlag {FAIL_DUP,
				IGNORE_DUP,
				DISCARD_DUP};
	protected:
		NodeIndexT nodeIndex;
		NodeIndexT& getIndex();
		map<Node*,EdgeSet*> inEdges;
		map<Node*,EdgeSet*> outEdges;
	public:
		friend class GraphIterator;
		Graph();
		~Graph();
		Graph(const Graph &);
		Graph& operator=(const Graph &);
		void clone(const Graph &);
		Edge* createEdge(Node*, Node*, Entity::EntityType, 
				Edge::CreateFlag flag=Edge::IGNORE_DUP);
		void deleteEdge(Node*, Node*, Entity::EntityType);
		bool hasEdge(Node*, Node*, Entity::EntityType=Entity::DEPENDS);
		Node* addNode(Node *node, AddFlag flag=FAIL_DUP);
		void deleteNode(const string &);
		Node* findNode(const string &) const;
		void freeEdges();
		EdgeSet& getInEdges(Node*);
		EdgeSet& getOutEdges(Node*);
		bool hasNode(const string &) const;
		void mergePackageVersions();
		GraphIterator& begin() const;
		GraphIterator& end() const;
		int size() const;
		string toGraphviz();
};

#endif /* GRAPH_H */
