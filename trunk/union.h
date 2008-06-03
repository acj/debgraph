#ifndef UNION_H
#define UNION_H

#include "dgoperator.h"
#include "graph.h"

using namespace std;

class Union : DGOperator {
	private:
		Graph &l_operand;
		Graph &r_operand;
		Graph result;
	protected:
	public:
		Union(Graph &, Graph &);
		~Union();
		Graph& execute();
};
#endif /* UNION_H */
