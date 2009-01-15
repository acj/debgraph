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
	public:
		~Edge();
		Edge();
		const Node *getFromNode() const;
		const Node *getToNode() const;
		void setFromNode(Node*);
		void setToNode(Node*);
};
		
#endif /* EDGE_H */
