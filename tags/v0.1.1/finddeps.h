#ifndef FINDDEPS_H
#define FINDDEPS_H

#include <set>
#include "dgunaryoperator.h"
#include "graph.h"

using namespace std;

class FindDeps : DGUnaryOperator {
	private:
		Node *singleNode;
		set<Node*> nodeList;
	public:
		FindDeps(Graph &g, Node *n) : DGUnaryOperator(g)
		{
			singleNode = n;
		};
		FindDeps(Graph &g, set<Node*> &v) : DGUnaryOperator(g)
		{
			nodeList = v;
		};
		~FindDeps();
		Graph& execute();
};
#endif /* FINDDEPS_H */
