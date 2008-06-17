#ifndef NODE_H
#define NODE_H

#include <string>
#include <set>
#include "entity.h"
#include "edge.h"

using namespace std;

typedef set<Edge*> EdgeSet;
typedef set<Edge*>::iterator EdgeSetIterator;
typedef set<Edge*>::const_iterator EdgeSetConstIterator;

class Graph;

class Node : public Entity {
	private:
		string id;
		Graph *parentGraph;
		EdgeSet outEdges;
		EdgeSet inEdges;
		int dfs;
		int low;
		int mark;
	public:
		Node();
		Node(Node&);
		Node(const string &nodeId);
		~Node();
		Node& operator=(Node&);
		const string& getId() const;
		Graph* getParentGraph() const;
		void setParentGraph(Graph*);
		void setId(string &nid);
		bool hasEdgeFrom(Node*) const;
		bool hasEdgeTo(Node*) const;
		void setDfs(int);
		int getDfs() const;
		void setLow(int);
		int getLow() const;
		void setMark(int);
		int getMark() const;
		EdgeSet& getInEdges();
		EdgeSet& getOutEdges();
		string toGraphviz();
};

#endif /* NODE_H */
