#include <ctype.h>

#include "vercmp.h"

/* the following is stolen from dpkg */
/* assume ascii; warning: evaluates x multiple times! */
#define order(x) ((x) == '~' ? -1 \
		: isdigit((x)) ? 0 \
		: !(x) ? 0 \
		: isalpha((x)) ? (x) \
		: (x) + 256)

static int verrevcmp(const char *val, const char *ref) {
  if (!val) val= "";
  if (!ref) ref= "";

  while (*val || *ref) {
    int first_diff= 0;

    while ( (*val && !isdigit(*val)) || (*ref && !isdigit(*ref)) ) {
      int vc= order(*val), rc= order(*ref);
      if (vc != rc) return vc - rc;
      val++; ref++;
    }

    while ( *val == '0' ) val++;
    while ( *ref == '0' ) ref++;
    while (isdigit(*val) && isdigit(*ref)) {
      if (!first_diff) first_diff= *val - *ref;
      val++; ref++;
    }
    if (isdigit(*val)) return 1;
    if (isdigit(*ref)) return -1;
    if (first_diff) return first_diff;
  }
  return 0;
}

int vercmp(const string &a, const string &b) {
	string aepoch, bepoch;
	string aversion, bversion;
	string arevision, brevision;
	int r;

	/* compare the epochs */
	string::size_type i = a.find(':', 0);
	if (i == string::npos) {
		aepoch = "0";
		aversion = a;
	}
	else {
		aepoch = a.substr(0, i);
		aversion = a.substr(i + 1, a.length());
	}
	i = b.find(':', 0);
	if (i == string::npos) {
		bepoch = "0";
		bversion = b;
	}
	else {
		bepoch = b.substr(0, i);
		bversion = b.substr(i + 1, b.length());
	}
	if ((r = verrevcmp(aepoch.c_str(), bepoch.c_str())) != 0)
		return r;

	/* compare the versions */
	i = aversion.rfind('-', a.length());
	if (i == string::npos) {
		arevision = "0";
	}
	else {
		arevision = aversion.substr(i + 1, aversion.length());
		aversion = aversion.substr(0, i);
	}
	i = bversion.rfind('-', b.length());
	if (i == string::npos) {
		brevision = "0";
	}
	else {
		brevision = bversion.substr(i + 1, bversion.length());
		bversion = bversion.substr(0, i);
	}
	if ((r = verrevcmp(aversion.c_str(), bversion.c_str())) != 0)
		return r;
	return verrevcmp(arevision.c_str(), brevision.c_str());
}
