#ifndef DGUNARYOPERATOR_H
#define DGUNARYOPERATOR_H

#include "dgoperator.h"
#include "graph.h"

using namespace std;

class DGUnaryOperator : public DGOperator {
	private:
	protected:
		Graph &operand;
		DGUnaryOperator(Graph &);
	public:
		virtual ~DGUnaryOperator();
		virtual Graph& execute() = 0;
		void copyConsistentEdges(Graph&, Graph&);
};

#endif /* DGUNARYOPERATOR_H */
