#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstring>

#include "debian.h"
#include "parser.h"

int dirwalker(string &dir, list<string> &flist) {
	DIR *dh;
	struct dirent *de;
	struct stat stat_buf;
	string dir2;

	// files first
	dh = opendir(dir.c_str());
	if (!dh) {
		return -1;
	}
	while ((de = readdir(dh)) != NULL) {
		/* ignore hidden, "." and ".." */
		if (de->d_name[0] == '.') {
			continue;
		}
		/* build the full path and check it */
		dir2 = dir + "/" + de->d_name;
		if (stat(dir2.c_str(), &stat_buf) == -1) {
			cerr << "Could not stat dir \"" <<
				dir2 << "\": " <<
				strerror(errno) << endl;
			closedir(dh);
			return -1;
		}
		if (S_ISREG(stat_buf.st_mode)) {	
			flist.push_back(dir2);
		}
	}
	closedir(dh);

// then subdirs
	dh = opendir(dir.c_str());
	if (!dh) {
		return -1;
	}
	while ((de = readdir(dh)) != NULL) {
		/* ignore hidden, "." and ".." */
		if (de->d_name[0] == '.') {
			continue;
		}
		/* build the full path and check it */
		dir2 = dir + "/" + de->d_name;
		if (stat(dir2.c_str(), &stat_buf) == -1) {
			cerr << "Could not stat dir \"" <<
				dir2 << "\": " <<
				strerror(errno) << endl;
			closedir(dh);
			return -1;
		}
		if (S_ISDIR(stat_buf.st_mode)) {
			if (dirwalker(dir2, flist) == -1) {
				closedir(dh);
				return -1;
			}
		}
	}
	closedir(dh);
	return flist.size();	
}

vector<string> breakPath(string path) {
	string::size_type pos;
	vector<string> result;
	while ((pos = path.find('/')) != string::npos) {
		result.push_back(path.substr(0, pos));
		path = path.substr(pos + 1, path.length());
	}
	result.push_back(path.substr(0, pos));
	return result;
}

void readFile(string &filename, Graph *g) {
	ifstream f;
	Edge *e;
	f.open(filename.c_str(), ios::in);
	// XXX check whether it worked
	// parse the filename
	vector<string> comps = breakPath(filename);
	if (comps.size() == 3) {
		// releases
		string release = comps[1];
		string type = comps[2];
		if (type == "Release") {
			cout << "reading " << filename << endl;
			ReleaseParser p(f);	
			Node *n;
			while ((n = p.getNext()) != NULL) {
				n = g->addNode(n, Graph::DISCARD_DUP);
			}
		}
		return;
	}
	if (comps.size() != 5) {
		/* ignore this non-std dir */
		f.close();
		return;
	}
	string release = comps[1];
	string component = comps[2];
	string arch = comps[3];
	string type = comps[4];
	// add the fixed nodes
	Node *releaseNode = new Node("Release:"+release);
	releaseNode->setType(Entity::RELEASE);
	releaseNode = g->addNode(releaseNode, Graph::DISCARD_DUP);
	Node *componentNameNode = new Node("ComponentName:"+component);
	componentNameNode->setType(Entity::COMPONENTNAME);
	componentNameNode = g->addNode(componentNameNode, Graph::DISCARD_DUP);
	Node *componentNode = new Node("Component:"+release+":"+component);
	componentNode->setType(Entity::COMPONENT);
	componentNode = g->addNode(componentNode, Graph::DISCARD_DUP);
	e = g->createEdge(releaseNode, componentNode, 
		Entity::CONTAINS, Edge::IGNORE_DUP);
	e = g->createEdge(componentNameNode, componentNode, 
		Entity::HAS_INSTANCE, Edge::IGNORE_DUP);

	if (type == "Sources") {
		cout << "reading " << filename << endl;
		SourcesParser p(f);	
		Node *n;
		while ((n = p.getNext()) != NULL) {
			n = g->addNode(n, Graph::DISCARD_DUP);
			// create the source name node 
			Node *name = new Node("SourceName:" 
				+ n->getProperty("Package"));
			name->setType(Entity::SOURCENAME);
			name = g->addNode(name, Graph::DISCARD_DUP);
			e = g->createEdge(name, n, Entity::HAS_VERSION, 
					Edge::NOCHECK_DUP);
			e = g->createEdge(componentNode, n, 
					Entity::CONTAINS, Edge::NOCHECK_DUP);
		}
	}
	else if (type == "Packages") {
		cout << "reading " << filename << endl;
		PackagesParser p(f);	
		Node *n;
		while ((n = p.getNext()) != NULL) {
			n = g->addNode(n, Graph::DISCARD_DUP);
			// create the source name node 
			Node *name = new Node("BinaryName:" 
				+ n->getProperty("Package"));
			name->setType(Entity::BINARYNAME);
			name->addProperty("Package", n->getProperty("Package"));
			name = g->addNode(name, Graph::DISCARD_DUP);
			e = g->createEdge(name, n, Entity::HAS_VERSION, Edge::NOCHECK_DUP);
			e = g->createEdge(componentNode, n, Entity::CONTAINS, 
					Edge::NOCHECK_DUP);
			Node *arch = new Node("Architecture:"
				+ n->getProperty("Architecture"));
			arch->setType(Entity::ARCHITECTURE);
			arch = g->addNode(arch, Graph::DISCARD_DUP);
			e = g->createEdge(n, arch, Entity::FOR_ARCHITECTURE, 
					Edge::NOCHECK_DUP);
		}
	}
	f.close();
}

void addDependency(Node *n, Graph *g, string dep, Entity::EntityType type) {
	string::size_type pos = dep.find('|', 0);
	if (pos != string::npos) {
		string d1 = dep.substr(0, pos);
		string d2 = dep.substr(pos + 1);
		Node *name;
		if (n->getType() != Entity::OR) {
			static int or_node_salt = 0;
			char salt[10];
			sprintf(salt, "%d", ++or_node_salt);
			string or_name("Or:");
			or_name += salt;
			name = new Node(or_name);
			name->addProperty("Package", "OR");
			name->setType(Entity::OR);
			name = g->addNode(name, Graph::IGNORE_DUP);
			g->createEdge(n, name, type);
		}
		else {
			name = n;
		}
		addDependency(name, g, d1, type);
		addDependency(name, g, d2, type);
	}
	else {
		while (dep[0] == ' ') {
			dep = dep.substr(1);
		}
		string bname;
		string spec = "";
		pos = 0;
		while ((dep[pos] != ' ') && (dep[pos] != '(') 
			&& (dep[pos] != '[') && (dep[pos] != '\0')) {
			pos++;
		}
		bname = dep.substr(0, pos);
		spec = dep.substr(pos);
		while (spec[0] == ' ') {
			spec = spec.substr(1);
		}
		Node *oNode = g->findNode("BinaryName:" + bname);
		if (oNode == NULL) {
			oNode = g->findNode("Virtual:" + bname);
		}
		if (oNode == NULL) {
			EdgeSet inEdges = g->getInEdges(n);
			while (n->getType() == Entity::OR) {
				if (inEdges.begin() != inEdges.end()) {
					Edge *e = *inEdges.begin();
					n = const_cast<Node*>(e->getFromNode());
				}
				else {
					break;
				}
			}
/*			cerr << "Relation (" << type << ") not found: " << bname 
				<< " (" << n->getId() << ")" <<endl;*/
			// XXX complain if the package with the missing dep is not in
			// non-free (contrib?)
			return;
		}
		else {
			g->createEdge(n, oNode, type);
		}
	}
}

void fixdeps(Node *n, Graph *g, string &d, Entity::EntityType type) {
	string::size_type it = 0;
	string::size_type itTokenEnd = 0;
	while(it < d.length())
	{
		while ((d[it] == ',') || (d[it] == ' ')) {
			it++;
		}
		itTokenEnd = d.find(",", it);
		if(it < itTokenEnd) {
			addDependency(n, g, 
				d.substr(it, itTokenEnd - it),
				type);
		}
		it = itTokenEnd;
	}	
}

void fixdeps(Node *n, Graph *g) {
	if (n->getType() == Entity::BINARY) {
		string d = n->getProperty("Depends");
		fixdeps(n, g, d, Entity::DEPENDS);
		d = n->getProperty("Pre-Depends");
		fixdeps(n, g, d, Entity::PRE_DEPENDS);
		d = n->getProperty("Conflicts");
		fixdeps(n, g, d, Entity::CONFLICTS);
		d = n->getProperty("Suggests");
		fixdeps(n, g, d, Entity::SUGGESTS);
		d = n->getProperty("Recommends");
		fixdeps(n, g, d, Entity::RECOMMENDS);
	}
}

void fixprovides(Node *n, Graph *g) {
	if (n->getType() == Entity::BINARY) {
		string d = n->getProperty("Provides");
		string::size_type it = 0;
		string::size_type itTokenEnd = 0;
		while(it < d.length())
		{
			while ((d[it] == ',') || (d[it] == ' ')) {
				it++;
			}
			itTokenEnd = d.find(",", it);
			if(it < itTokenEnd) {
				Node *name = new Node("Virtual:" 
					+ d.substr(it, itTokenEnd - it));
				name->setType(Entity::VIRTUAL);
				name = g->addNode(name, Graph::DISCARD_DUP);
				g->createEdge(name, n, Entity::PROVIDED);
			}
			it = itTokenEnd;
		}	
	}	
}

void fixArchitecture(Node *n, Graph *g) {
	if (n->getType() == Entity::SOURCE) {
		string d = n->getProperty("Architecture");
		string::size_type it = 0;
		string::size_type itTokenEnd = 0;
		while(it < d.length())
		{
			while (d[it] == ' ') {
				it++;
			}
			itTokenEnd = d.find(" ", it);
			if(it < itTokenEnd) {
				Node *arch = new Node("Architecture:"
					+ d.substr(it, itTokenEnd - it));
				arch->setType(Entity::ARCHITECTURE);
				arch = g->addNode(arch, Graph::DISCARD_DUP);
				g->createEdge(n, arch, Entity::FOR_ARCHITECTURE, 
					Edge::IGNORE_DUP);
			}
			it = itTokenEnd;
		}
	}
}

void fixRelease(Node *n, Graph *g) {
	if (n->getType() == Entity::RELEASE) {
		string d = n->getProperty("Architectures");
		string::size_type it = 0;
		string::size_type itTokenEnd = 0;
		while(it < d.length())
		{
			while (d[it] == ' ') {
				it++;
			}
			itTokenEnd = d.find(" ", it);
			if(it < itTokenEnd) {
				Node *arch = new Node("Architecture:"
					+ d.substr(it, itTokenEnd - it));
				arch->setType(Entity::ARCHITECTURE);
				arch = g->addNode(arch, Graph::DISCARD_DUP);
				g->createEdge(n, arch, Entity::INCLUDES_ARCH, 
					Edge::IGNORE_DUP);
			}
			it = itTokenEnd;
		}
	}
}

DebianGraph::DebianGraph(string cachedir) {
	list<string> filelist;
	// read in the files	
	dirwalker(cachedir, filelist);
	list<string>::iterator it = filelist.begin();
	while (it != filelist.end()) {
		readFile(*it, this);
		it++;
	}
	multimap<string, Node*> nIndex = this->getIndex();
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
		fixprovides(n, this);
	}
	// fix the dependencies and architectures
	for (multimap<string, Node*>::iterator i = nIndex.begin();
		i != nIndex.end(); i++) {
		Node *n = i->second;
		fixdeps(n, this);
		fixArchitecture(n, this);
		fixRelease(n, this);
	}
}

DebianGraph::~DebianGraph() {
}

void DebianGraph::freeNodes() {
	Node *n;
	for (NodeIndexT::iterator i=nodeIndex.begin(); i != nodeIndex.end(); ++i) {
		n = (*i).second;
		nodeIndex.erase(nodeIndex.begin());
		delete(n);
	}
}
