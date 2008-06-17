#ifndef FINDCYCLESTEST_H
#define FINDCYCLESTEST_H

#include "testkit.h"

class FindCyclesTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "FindCyclesTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new FindCyclesTest());

#endif /* FINDCYCLESTEST_H */
