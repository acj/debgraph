#ifndef NODE_H
#define NODE_H

#include <string>
#include <set>
#include "entity.h"
#include "edge.h"

using namespace std;

class Graph;

class Node : public Entity {
	private:
		string id;
		Graph *parentGraph;
		set<Edge*> outEdges;
		set<Edge*> inEdges;
		int dfs;
		int low;
		int mark;
	public:
		Node();
		Node(Node&);
		Node(const string &nodeId);
		~Node();
		Node& operator=(Node&);
		const string& getId();
		Graph* getParentGraph();
		void setParentGraph(Graph* g);
		void setId(string &nid);
		void setDfs(int d);
		int getDfs();
		void setLow(int l);
		int getLow();
		void setMark(int m);
		int getMark();
		set<Edge*>& getInEdges();
		set<Edge*>& getOutEdges();
		string toGraphviz();
};

#endif /* NODE_H */
