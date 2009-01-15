#include <iostream>
#include <cstring>
#include "parser.h"

Parser::Parser(istream &is) 
	: file(is) {
}

Parser::~Parser() {
}

ContinuationLineParser::ContinuationLineParser(istream &is) : Parser(is) {
	hasLine = false;
}

void ContinuationLineParser::readLine() {
	if (file.peek() == EOF) {
		hasLine = false;
		return;
	}
	line = "";
	if (file.peek() == '\n') {
		hasLine = true;
		file.get();
		return;
	}
	do {
		do {
			file.get(buffer, BUFSIZE, '\n');
			line += buffer;
		} while ((strlen(buffer) >= (BUFSIZE-1)) 
			&& (file.peek() != '\n'));
		file.get();
	} while(file.peek() == ' ');
	hasLine = true;
}

bool ContinuationLineParser::hasNextLine() {
	if (!hasLine) {
		readLine();
	}
	return hasLine;
}

string& ContinuationLineParser::getNextLine() {
	if (!hasLine) {
		readLine();
	}
	hasLine = false;
	return line;
}

Node* ContinuationLineParser::getNext() {
	Node *node = new Node();
	while (hasNextLine()) {
		string line = getNextLine();
		if (line == "") {
			/* make sure the node actually contains anything */
			if (node->getProperties().size() > 0) {
				return node;
			}
			else {
				delete node;
				return NULL;
			}
		}
		else {
			string::size_type pos = line.find(":", 0);
			if (pos != string::npos) {
				string key = line.substr(0, pos);
				pos++;
				while (line[pos] == ' ') {
					pos++;
				}
				string val = line.substr(pos, line.length());
				// blacklisting some useless but memory-intesive fields
				if ((key.compare("Description") != 0) 
					&& (key.compare("MD5Sum") != 0)
					&& (key.compare("SHA1") != 0)
					&& (key.compare("Filename") != 0)
					&& (key.compare("Installed-Size") != 0)
					&& (key.compare("Maintainer") != 0)
					&& (key.compare("Size") != 0)
					&& (key.compare("SHA256") != 0)) {
					node->replaceProperty(key, val);
				}
			}
			else {
			}
		}
	}
	/* make sure the node actually contains anything */
	if (node->getProperties().size() > 0) {
		return node;
	}
	else {
		delete node;
		return NULL;
	}
}

SourcesParser::SourcesParser(istream &is) : ContinuationLineParser(is) {
}

Node* SourcesParser::getNext() {
	Node *n = ContinuationLineParser::getNext();
	if (n != NULL) {
		if ((!n->hasProperty("Package"))
			|| (!n->hasProperty("Version"))) {
			throw "missing property";
		}
		string idx = "Source:" + n->getProperty("Package") + ":" 
			+ n->getProperty("Version");
		n->setId(idx);
		n->setType(Entity::SOURCE);
	}
	return n;
}

PackagesParser::PackagesParser(istream &is) : ContinuationLineParser(is) {
}

Node* PackagesParser::getNext() {
	Node *n = ContinuationLineParser::getNext();
	if (n != NULL) {
		if ((!n->hasProperty("Package"))
			|| (!n->hasProperty("Version"))
			|| (!n->hasProperty("Architecture"))) {
			throw "missing property";
		}
		string idx = "Binary:" + n->getProperty("Package") + ":" 
			+ n->getProperty("Version") + ":" + n->getProperty("Architecture");
		n->setId(idx);
		n->setType(Entity::BINARY);
	}
	return n;
}

ReleaseParser::ReleaseParser(istream &is) : ContinuationLineParser(is) {
}

Node* ReleaseParser::getNext() {
	Node *n = ContinuationLineParser::getNext();
	if (n != NULL) {
		if ((!n->hasProperty("Architectures")) 
			|| (!n->hasProperty("Suite"))) {
			throw "missing property";
		}
		string idx = "Release:" + n->getProperty("Suite");
		n->setId(idx);
		n->setType(Entity::RELEASE);
	}
	return n;
}
