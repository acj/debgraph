#ifndef FILTERTEST_H
#define FILTERTEST_H

#include "testkit.h"

class FilterTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "FilterTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new FilterTest());

#endif /* FILTERTEST_H */
