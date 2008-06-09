#ifndef DIFFERENCETEST_H
#define DIFFERENCETEST_H

#include "testkit.h"

class DifferenceTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "DifferenceTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new DifferenceTest());

#endif /* DIFFERENCETEST_H */
