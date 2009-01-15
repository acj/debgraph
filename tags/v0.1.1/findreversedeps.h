#ifndef FINDREVERSEDEPS_H
#define FINDREVERSEDEPS_H

#include <set>
#include "dgunaryoperator.h"
#include "graph.h"

using namespace std;

class FindReverseDeps : DGUnaryOperator {
	private:
		set<Node*> nodeList;
	public:
		FindReverseDeps(Graph &g, Node *n) : DGUnaryOperator(g)
		{
			nodeList.insert(n);
		};
		FindReverseDeps(Graph &g, set<Node*> &v) : DGUnaryOperator(g)
		{
			nodeList = v;
		};
		~FindReverseDeps();
		Graph& execute();
};
#endif /* FINDREVERSEDEPS_H */
