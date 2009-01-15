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
};

#endif /* NODE_H */
