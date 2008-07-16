#ifndef DGOPERATOR_H
#define DGOPERATOR_H

#include "graph.h"
#include "memacct.h"

using namespace std;

class DGOperator {
	private:
	protected:
		DGOperator();
	public:
		virtual ~DGOperator();
		virtual Graph& execute() = 0;
		void registerObject(void*);
};

#endif /* DGOPERATOR_H */
