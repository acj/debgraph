#ifndef NODETEST_H
#define NODETEST_H

#include "testkit.h"

class NodeTest : public Test {
	public:
		bool run(Graph &g);
		inline string getName() { return "NodeTest"; }
};

static TestKit *_tk = TestKit::instance()->addTest(new NodeTest());

#endif /* NODETEST_H */
