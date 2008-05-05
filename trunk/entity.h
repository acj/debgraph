#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <map>
#include <set>

using namespace std;

class Entity {
	public:
		/* if you change this, modify getTypeString() in entity.cc accordingly
		 * */
		enum EntityType {NONE, 		/* invalid value */
				SOURCE, 	/* a source package */
				SOURCENAME,	/* a source package name */
				BINARY,
				VIRTUAL,
				BINARYNAME,
				RELEASE,
				COMPONENTNAME,
				COMPONENT,
				OR,
				ARCHITECTURE,
				
				HAS_VERSION,	
				CONTAINS,	
				HAS_INSTANCE,
				DEPENDS,
				PRE_DEPENDS,
				CONFLICTS,
				SUGGESTS,
				RECOMMENDS,
				PROVIDED,
				FOR_ARCHITECTURE,
				INCLUDES_ARCH
				};
	protected:
		EntityType type;
		map<string, string> properties;	
		static set<string> stringcache;
	public:
		void printcache();
		Entity();
		void addProperty(const string &key, const string &value);
		void replaceProperty(const string &key, const string &value);
		const string& getProperty(const string &key);
		bool hasProperty(const string &key);
		map<string, string>& getProperties();
		EntityType getType();
		void setType(EntityType eType);
		string getTypeString();
};

#endif /* ENTITY_H */
