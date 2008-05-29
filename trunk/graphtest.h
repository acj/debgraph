#ifndef GRAPHTEST_H
#define GRAPHTEST_H

#include "testkit.h"

class GraphTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "GraphTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new GraphTest());

#endif /* GRAPHTEST_H */
