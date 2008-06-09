#include <iostream>
#include <fstream>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include "testkit.h"
#include "debian.h"

int main(int argc, char *argv[]) {
	time_t ct = time(NULL);
	ofstream ofile;
	ofile.open("test/out/index.html");

	ofile << "<h1>Unit Test Index</h1>" << endl;
	DebianGraph g("../cache");
	ofile << endl;

	if (TestKit::instance()->run(g) == false) {
		cout << "At least one test failed!" << endl;
	}
	else {
		cout << "All tests passed." << endl;
	}

	TestKit::instance()->createIndex(ofile);

	ofile << "<br><font size=\"-1\">Problems? Robert Lemmen &lt;<a href=\"mailtourobertle@semistable.com\">robertle@semistable.com</a>&gt;</font>" << endl;
	ofile << "<br><font size=\"-1\">Generated " << ctime(&ct) << "</font>" << endl;
	ofile.close();

	return 0;
}
