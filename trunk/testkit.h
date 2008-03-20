#ifndef TESTKIT_H
#define TESTKIT_H

#include <list>

#include "graph.h"

using namespace std;

class TestKit;

class MenuEntry {
	protected:
		string menuPath;
		string comment;
		string filename;
	public:
		MenuEntry(string menuPath, string comment, string filename);
		string getMenuPath();
		string getComment();
		string getFilename();
};

class Test {
	friend class TestKit;
	protected:
		TestKit *tk;
		void addItem(string filename, string menuPath, string comment);
	public:
		virtual string getName() = 0;
		virtual void run(Graph &g) = 0;
		virtual ~Test();
};

class TestKit {
	friend class Test;
		static TestKit *tk;
		TestKit();
		~TestKit();
		list<Test*> tests;
	protected:
		void addItem(string filename, string menuPath, string comment);
		list<MenuEntry*> menuList;
	public:
		static TestKit* instance();
		TestKit *addTest(Test *t);
		void run(Graph &g);
		void createIndex(ostream &of);
};


#endif /* TESTKIT_H */
