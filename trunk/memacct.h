#ifndef MEMACCT_H
#define MEMACCT_H

#include <set>

using namespace std;

class MemAcct {
		static MemAcct *ma;
		MemAcct();
		~MemAcct();
		set<void*> objectList;
	protected:
	public:
		static MemAcct* instance();
		void addReference(void*);
		void deleteReference(void*);
};


#endif /* MEMACCT_H */
