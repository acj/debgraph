#ifndef FINDREVERSEDEPSTEST_H
#define FINDREVERSEDEPSTEST_H

#include "testkit.h"

class FindReverseDepsTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "FindReverseDepsTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new FindReverseDepsTest());

#endif /* FINDREVERSEDEPSTEST_H */
