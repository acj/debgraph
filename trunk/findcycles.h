#ifndef FINDCYCLES_H
#define FINDCYCLES_H

#include <list>

#include "testkit.h"

class FindCycles: public Test {
	protected:
		int tarjan(Graph *g, Node *n, list<Node*> *l, int *N, 
			set<Entity::EntityType> &ae, string dist);
		void pre_dep(Graph &g, string release);
		void norm_dep(Graph &g, string release);
		list<string> ssource;
		int ccount;
		list<string> packages;
	public:
		void run(Graph &g);
		virtual ~FindCycles();
};

static TestKit *_tk = TestKit::instance()->addTest(new FindCycles());

#endif /* FINDCYCLES_H */
