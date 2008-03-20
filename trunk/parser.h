#ifndef PARSER_H
#define PARSER_H

#include <istream>

#include "node.h"

using namespace std;

class Parser {
	protected:
		istream &file;
	public:
		Parser(istream &is);
		virtual ~Parser();
		virtual Node *getNext() = 0;
};

class ContinuationLineParser : public Parser {
		static const int BUFSIZE = 4096;
		char buffer[BUFSIZE];
		string line;
		bool hasLine;
		void readLine();
	public:
		bool hasNextLine();
		string& getNextLine();
	public:
		ContinuationLineParser(istream &is);
		Node *getNext();
};

class SourcesParser: public ContinuationLineParser {
	public:
		SourcesParser(istream &is);
		Node *getNext();
};

class PackagesParser: public ContinuationLineParser {
	public:
		PackagesParser(istream &is);
		Node *getNext();
};

class ReleaseParser: public ContinuationLineParser {
	public:
		ReleaseParser(istream &is);
		Node *getNext();
};

#endif /* PARSER_H */
