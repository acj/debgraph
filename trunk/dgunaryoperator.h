#ifndef DGUNARYOPERATOR_H
#define DGUNARYOPERATOR_H

#include "dgoperator.h"
#include "graph.h"

using namespace std;

class DGUnaryOperator : public DGOperator {
	private:
	protected:
		Graph &operand;
		Graph result;
		DGUnaryOperator(Graph &);
	public:
		virtual ~DGUnaryOperator();
		virtual Graph& execute() = 0;
		void addConsistentEdges();
};

#endif /* DGUNARYOPERATOR_H */
