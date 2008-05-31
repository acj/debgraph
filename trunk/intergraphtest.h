#ifndef INTERGRAPHTEST_H
#define INTERGRAPHTEST_H

#include "testkit.h"

class InterGraphTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "InterGraphTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new InterGraphTest());

#endif /* INTERGRAPHTEST_H */
