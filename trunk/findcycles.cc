#include <iostream>
#include <fstream>

using namespace std;

#include "findcycles.h"

bool NodeNameComparator(const Node *a, const Node *b) {
	return (const_cast<Node*>(a))->getProperty("Package") < 
			(const_cast<Node*>(b))->getProperty("Package");
}

FindCycles::~FindCycles() {
}

static inline int min(int a, int b) {
	return a < b ? a : b;
}

string hex(void *p) {
	char buf[67];
	string s;
	sprintf(buf, "X%08X", (unsigned int)p);
	s = buf;
	return s;
}

string sourcePackage(Node *p) {
	set<Edge*> e = p->getOutEdges();
	set<Edge*>::iterator ei;
	for (ei = e.begin(); ei != e.end(); ei++) {
		Node *on = const_cast<Node*>((*ei)->getToNode());
		if (on->getType() == Entity::SOURCE) {
			return on->getProperty("Package");
		}
	}
	return "";
}

int printGraph(list<Node*> &l, ostream &os = cout) {
	int pdc = 0;
	os << "digraph {" << endl
		<< "node [shape=rect];" << endl
		<< "edge [arrowhead=onormal];" << endl;
	list<Node*>::iterator i;
	map<Node*, Node*> replace;
	set<Node*> ls;
	for (i = l.begin(); i != l.end(); i++) {
		ls.insert(ls.begin(), *i);
		if ((*i)->getType() == Entity::BINARYNAME) {
			/* find the corresponding binary */
			list<Node*>::iterator j;
			for (j = l.begin(); j != l.end(); j++) {
				if ((*j)->getType() == Entity::BINARY) {
					string a = (*j)->getProperty("Package");
					string b =(*i)->getProperty("Package");
					if (a == b) {
						replace.insert(
						pair<Node*, Node*>(*j, *i));
					}
				}
			}
		}
	}
	for (i = l.begin(); i != l.end(); i++) {
		set<Edge*> e = (*i)->getOutEdges();
		set<Edge*>::iterator ei;
		for (ei = e.begin(); ei != e.end(); ei++) {
			Node *on = const_cast<Node*>((*ei)->getToNode());
			if (ls.find(on) == ls.end()) {
				continue;
			}
			if (((*ei)->getType() != Entity::DEPENDS) &&
				((*ei)->getType() != Entity::PRE_DEPENDS)) {
				continue;
			}
			/* check if we need to replace this */
			if (replace.find(on) != replace.end()) {
				on = replace.find(on)->second;
			}
			string a = (*i)->getProperty("Package");
			if (a == "") {
				a = hex(*i);
				if ((*i)->getType() == Entity::OR) {
					os << a << "[shape=circle,label=\"OR\"];" << endl;
				}
				else {
					os << a << "[shape=circle,label=\"\"];" << endl;
				}
			}
			unsigned int p;
			while ((p = a.find("-")) != string::npos) {
				a.replace(p,1,"_");
			}
			while ((p = a.find(".")) != string::npos) {
				a.replace(p,1,"_");
			}
			while ((p = a.find("+")) != string::npos) {
				a.replace(p,1,"_");
			}
			string b = on->getProperty("Package");
			if (b == "") {
				b = hex(on);
			}
			while ((p = b.find("-")) != string::npos) {
				b.replace(p,1,"_");
			}
			while ((p = b.find(".")) != string::npos) {
				b.replace(p,1,"_");
			}
			while ((p = b.find("+")) != string::npos) {
				b.replace(p,1,"_");
			}
			if ((*ei)->getType() == Entity::PRE_DEPENDS) {
				pdc++;
			}
		/*	os << "\t" << a << " -> " << b << " [label=";
			switch ((*ei)->getType()) {
				case Entity::PRE_DEPENDS:
					os << "Pre-Depends";
					break;
				case Entity::DEPENDS:
					os << "Depends";
					break;
				default:
					os << "\"\"";
			}
			os << "];" << endl;*/
			os << "\t" << a << " -> " << b << ";" << endl;
		}
	}
	os << "}" << endl;
	return pdc;
}

int FindCycles::tarjan(Graph *g, Node *n, 
	list<Node*> *l, int *N, set<Entity::EntityType> &ae, string dist) {
	l->push_back(n);
	n->setDfs(*N);
	n->setLow(*N);
	(*N)++;
	int retval = 0;

	/* recursion */
	set<Edge*> s = n->getOutEdges();
	set<Edge*>::iterator i;
	for (i = s.begin(); i != s.end(); i++) {
		/* examine this link */
		if (ae.find((*i)->getType()) != ae.end()) {
			Node *on = const_cast<Node*>((*i)->getToNode());
			if (on->getMark() == 0) {
				on->setMark(1);
				retval += tarjan(g, on, l, N, ae, dist);
				n->setLow(min(n->getLow(), on->getLow()));
			}
			else if (on->getMark() == 1) {
				n->setLow(min(n->getLow(), on->getDfs()));
			}
		}
	}

	if (n->getLow() == n->getDfs()) {
		Node *cn;
		list<Node*> tl;
		int bc = 0;
		do {
			cn = l->back();
			l->pop_back();
			cn->setMark(2);
			if (cn->getType() == Entity::BINARY) {
				bc++;
			}
			tl.push_back(cn);
		} while (cn != n);
		if (bc > 1) {
			/* XXX sort tl to make this more stable */
			tl.sort(NodeNameComparator);
			list<Node*>::iterator i;
			cout << "* ";
			string filename = "";
			string filename2;
			string line;
			for (i = tl.begin(); i != tl.end(); i++) {
				if ((*i)->getType() == Entity::BINARY) {
					ssource.push_back((*i)->getProperty("Maintainer") + "\t" + 
									(*i)->getProperty("Package"));
					/* XXX filename misses release */
					if (filename < (*i)->getProperty("Package")) {
						
						filename = (*i)->getProperty("Package");
					}
					line += (*i)->getProperty("Package")
						+ " ";
				}
/*			if ((pnode) || (! (sourcePackage(pnode) 
					== sourcePackage(*i)))) {
					is_single_source = false;	
				}
				pnode = (*i);*/
			}
		/*	if (is_single_source) {
				ssource.push_back(sourcePackage(pnode));
			}*/
			packages.push_back(line);
			filename2 = "dot/" + filename + "_" + dist + ".png";
			cout << "<a href=\"" + filename2 + "\">" + line + "</a>" ;
			//cout << line;
			filename2 = "dot/" + filename + "_" + dist + ".dot";
			ofstream of;
			of.open(filename2.c_str());
			int pdc = printGraph(tl, of);
			ccount++;
			of.close();
			//cout << "(" << pdc << " pre-depends)<br>" << endl;
			if (pdc > 0) {
				cout << " (" << pdc << " pre-depends)<br>\n" << endl;
			}
			else {
				cout << "<br>\n";
			}
			retval++;
		}
	}
	return retval;
}

void mark_rec(Node *n, int depth, int mark) {
	if (depth == 0) {
		return;
	}
	if (n->getType() != Entity::OR) {
		depth--;
	}
	n->setMark(mark);
	set<Edge*> s = n->getOutEdges();
	set<Edge*>::iterator i;
	for (i = s.begin(); i != s.end(); i++) {
		Node *on = const_cast<Node*>((*i)->getToNode());
		mark_rec(on, depth, mark);
	}
}

void FindCycles::pre_dep(Graph &g, string release) {
	int N = 0;
	list<Node*> l;
	set<Entity::EntityType> ae;
	ae.insert(Entity::CONTAINS);
	ae.insert(Entity::PRE_DEPENDS);
	ae.insert(Entity::HAS_VERSION);

	/* mark all binaries as handled */
	multimap<string, Node*> nIndex = g.getIndex();
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
		if (n->getType() == Entity::BINARY) {
			n->setMark(2);
		}
		else {
			n->setMark(0);
		}
	}
	cout << "<h2>Cyclic Pre-Depends (" << release << ")</h2>" << endl;
	Node *n = g.findNode("Release:"+release);
	if (n == NULL) {
		cout << "Error: Release \"" << release << 
		"\" not found in graph<br>" << endl;
	}
	else {
		/* mark as unhadled in our release  */
		mark_rec(n, 4, 0);
		if (tarjan(&g, n, &l, &N, ae, release) == 0) {
			cout << "none<br>" << endl;
		}
	}
	cout << endl;
}

void FindCycles::norm_dep(Graph &g, string release) {
	int N = 0;
	list<Node*> l;
	set<Entity::EntityType> ae;
	ae.insert(Entity::CONTAINS);
	ae.insert(Entity::PRE_DEPENDS);
	ae.insert(Entity::DEPENDS);
	ae.insert(Entity::HAS_VERSION);

	/* mark all binaries as handled */
	multimap<string, Node*> nIndex = g.getIndex();
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
		if (n->getType() == Entity::BINARY) {
			n->setMark(2);
		}
		else {
			n->setMark(0);
		}
	}
	cout << "<h2>Cyclic Depends (" << release << ")</h2>" << endl;
	/* XXX dirty */
	if (release == "unstable") {
		cout << "[<a href=\"progress.png\">Progress</a>] " << endl;
		cout << "[<a href=\"unstable_diffs.txt\">Diffs</a>] " << endl;
		cout << "[<a href=\"unstable_developers.txt\">Developers</a>] " << endl;
		cout << "<br/><br/>" << endl;
	}
	Node *n = g.findNode("Release:"+release);
	if (n == NULL) {
		cout << "Error: Release \"" << release << 
		"\" not found in graph<br>" << endl;
	}
	else {
		/* mark as unhadled in our release  */
		mark_rec(n, 3, 0);
		if (tarjan(&g, n, &l, &N, ae, release) == 0) {
			cout << "none<br>" << endl;
		}
	}
	cout << endl;
}

void FindCycles::run(Graph &g) {

	cout << "<h1>CYCLICTEST</h1>" << endl;

/*	pre_dep(g, "stable");
	pre_dep(g, "testing");
	pre_dep(g, "unstable");*/
	norm_dep(g, "stable");
	ssource.unique();
	ofstream of;
	of.open("stable_packages.txt");
	for (list<string>::iterator it = ssource.begin();
		it != ssource.end(); it++) {
		of << *it << endl;
	}
	of.close();
	ssource.clear();
	norm_dep(g, "testing");
	ssource.unique();
	of.open("testing_packages.txt");
	for (list<string>::iterator it = ssource.begin();
		it != ssource.end(); it++) {
		of << *it << endl;
	}
	of.close();
	ssource.clear();
	packages.clear();
	ccount = 0;
	norm_dep(g, "unstable");
	ssource.unique();
	of.open("unstable_packages.txt");
	for (list<string>::iterator it = ssource.begin();
		it != ssource.end(); it++) {
		of << *it << endl;
	}
	time_t ct;
	time(&ct);
	struct tm *st;
	st = localtime(&ct);
	char buffer[100];
	strftime(buffer, 100, "%d.%m.%Y", st);
	of.close();
	string date = buffer;
	of.open("unstable_progress.txt", ofstream::out | ofstream::app);
		of << date << "\t" << ccount << endl;
	of.close();
	packages.sort();
	of.open("unstable_raw.txt", ofstream::out | ofstream::trunc);
	for (list<string>::iterator it = packages.begin();
		it != packages.end(); it++) {
		of << *it << endl;
	}
	of.close();
	
	multimap<string, Node*> nIndex = g.getIndex();
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
		n->setMark(0);
	}

	cout << endl;
}
