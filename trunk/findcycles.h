#ifndef FINDCYCLES_H
#define FINDCYCLES_H

#include <list>
#include <vector>
#include "dgunaryoperator.h"
#include "graph.h"

class FindCycles: public DGUnaryOperator {
	private:
		vector<Graph> cycles;
	protected:
		int tarjan(Graph *g, Node *n, list<Node*> *l, int *N, 
			set<Entity::EntityType> &ae, string dist);
		void pre_dep(Graph &g, string release);
		void norm_dep(Graph &g, string release);
		list<string> ssource;
		int ccount;
		list<string> packages;
	public:
		vector<Graph>& getCycles();
		Graph& execute();
		FindCycles(Graph &g) : DGUnaryOperator(g) { };
		~FindCycles();
};

#endif /* FINDCYCLES_H */
