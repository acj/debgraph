#ifndef EDGE_H
#define EDGE_H

#include "entity.h"

using namespace std;

class Node;

class Edge : public Entity {
	public:
		enum CreateFlag {IGNORE_DUP, NOCHECK_DUP};
	protected:
		Node *from;
		Node *to;
		Edge();
	public:
		~Edge();
		static Edge *createEdge(Node *fromNode, Node *toNode, 
			Entity::EntityType type, CreateFlag flag = NOCHECK_DUP);
		const Node *getFromNode();
		const Node *getToNode();
};
		
#endif /* EDGE_H */
