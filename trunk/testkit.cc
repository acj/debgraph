#include "testkit.h"

#include <iostream>
		
MenuEntry::MenuEntry(string filename, string menuPath, string comment) {
	this->menuPath = menuPath;
	this->comment = comment;
	this->filename = filename;
}
		
string MenuEntry::getMenuPath() {
	return menuPath;
}

string MenuEntry::getComment() {
	return comment;
}

string MenuEntry::getFilename() {
	return filename;
}

TestKit* TestKit::tk = 0;

TestKit::TestKit() {
}

TestKit::~TestKit() {
}

Test::~Test() {
}

TestKit* TestKit::instance() {
	if (tk == 0) {
		tk = new TestKit();
	}
	return tk;
}

TestKit* TestKit::addTest(Test *t) {
	t->tk = this;
	tests.push_back(t);
	return this;
}

void TestKit::run(Graph &g) {
	list<Test*>::iterator i;
	for (i = tests.begin(); i != tests.end(); i++) {
		cout << "Running " << (*i)->getName() << "..." << endl;
		(*i)->run(g);
	}
	
}
		
void Test::addItem(string filename, string menuPath, string comment) {
	tk->addItem(filename, menuPath, comment);
}

void TestKit::addItem(string filename, string menuPath, string comment) {
	menuList.push_back(new MenuEntry(filename, menuPath, comment));	
}

void TestKit::createIndex(ostream &of) {
	// XXX take menu structure into account
	for (list<MenuEntry*>::iterator i = menuList.begin();
		i != menuList.end(); i++) {
		of << "<a href=\"" << (*i)->getFilename() << "\">" 
			<< (*i)->getComment()
			<< "</a><br>" << endl;
	}
}
