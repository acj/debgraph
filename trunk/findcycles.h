#ifndef FINDCYCLES_H
#define FINDCYCLES_H

#include <vector>
#include "dgunaryoperator.h"
#include "graph.h"

using namespace std;

class FindCycles : DGUnaryOperator {
	private:
		vector<Graph> cycles;
		set<string> visitedNodes;
		vector<Node*> depVector;
	public:
		FindCycles(Graph &g) : DGUnaryOperator(g) { };
		~FindCycles();
		Graph& execute();
		vector<Graph>& getCycles();
		void findCycles(Node*);
};
#endif /* FINDCYCLES_H */
