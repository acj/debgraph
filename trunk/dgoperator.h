#ifndef DGOPERATOR_H
#define DGOPERATOR_H

#include "node.h"
#include "graph.h"

using namespace std;

class DGOperator {
	private:
		Graph result;
	protected:
		DGOperator();
	public:
		virtual ~DGOperator();
		virtual Graph& execute();
};

#endif /* DGOPERATOR_H */
