#ifndef TYPES_H
#define TYPES_H

#include "node.h"

/* would be cool if this could be a hash_multimap */
typedef multimap<string, Node*> GraphIndexT;
typedef multimap<string, Node*>::iterator GraphIndexIteratorT;
typedef multimap<string, Node*>::const_iterator GraphIndexConstIteratorT;

#endif /* TYPES_H */
