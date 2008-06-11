#ifndef FINDDEPSTEST_H
#define FINDDEPSTEST_H

#include "testkit.h"

class FindDepsTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "FindDepsTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new FindDepsTest());

#endif /* FINDDEPSTEST_H */
