#ifndef TYPES_H
#define TYPES_H

#include "node.h"

/* would be cool if this could be a hash_multimap */
typedef multimap<string, Node*> NodeIndexT;
typedef multimap<string, Node*>::iterator NodeIndexIteratorT;
typedef multimap<string, Node*>::const_iterator NodeIndexConstIteratorT;

#endif /* TYPES_H */
