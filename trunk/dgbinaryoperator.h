#ifndef DGBINARYOPERATOR_H
#define DGBINARYOPERATOR_H

#include "dgoperator.h"
#include "node.h"
#include "graph.h"

using namespace std;

class DGBinaryOperator : public DGOperator {
	private:
	protected:
		Graph &l_operand;
		Graph &r_operand;
		DGBinaryOperator(Graph &, Graph &);
	public:
		virtual ~DGBinaryOperator();
		virtual Graph& execute() = 0;
		void copyConsistentEdges(Graph&, Graph&, Graph&);
};

#endif /* DGBINARYOPERATOR_H */
