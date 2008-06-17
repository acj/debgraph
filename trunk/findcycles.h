#ifndef FINDCYCLES_H
#define FINDCYCLES_H

#include <vector>
#include "dgunaryoperator.h"
#include "graph.h"

using namespace std;

class FindCycles : DGUnaryOperator {
	private:
		vector<Graph> cycles;
		size_t cycle_limit;
	public:
		FindCycles(Graph &g, size_t max_cycles = 0) : DGUnaryOperator(g)
		{
			cycle_limit = max_cycles;
		};
		~FindCycles();
		Graph& execute();
		vector<Graph>& getCycles();
};
#endif /* FINDCYCLES_H */
