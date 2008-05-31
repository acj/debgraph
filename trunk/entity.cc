#include <iostream>

#include "entity.h"

set<string> Entity::stringcache;

void Entity::printcache() {
	set<string>::iterator i;
	for (i = stringcache.begin(); i != stringcache.end();  i++) {
		cout << *i << endl;
	}
}

Entity::Entity() {
	type = NONE;
}

Entity::~Entity() {
}

void Entity::addProperty(const string &key, const string &value) {
	if (!hasProperty(key)) {
		replaceProperty(key, value);
	}
}

void Entity::replaceProperty(const string &key, const string &value) {
	set<string>::iterator i, j;
	if ((i = stringcache.find(key)) != stringcache.end()) {
		if ((j = stringcache.find(value)) != stringcache.end()) {
			properties[*i] = *j;		
		}
		j = stringcache.insert(value).first;
		properties[*i] = *j;		
	}
	else {
		i = stringcache.insert(key).first;
		if ((j = stringcache.find(value)) != stringcache.end()) {
			properties[*i] = *j;		
		}
		j = stringcache.insert(value).first;
		properties[*i] = *j;		
	}
}

const string &Entity::getProperty(const string &key) {
	return properties[key];
}
		
bool Entity::hasProperty(const string &key) {
	map<string, string>::iterator i = properties.find(key);
	return (i != properties.end());
}

map<string, string>& Entity::getProperties() {
	return properties;
}
		
Entity::EntityType Entity::getType() {
	return type;
}

void Entity::setType(Entity::EntityType eType) {
	type = eType;
}
		
string Entity::getTypeString() {
	switch (type) {
		case Entity::NONE:
			return "NONE";
		case Entity::SOURCE:
			return "SOURCE";
		case Entity::SOURCENAME:
			return "SOURCENAME";
		case Entity::BINARY:
			return "BINARY";
		case Entity::VIRTUAL:
			return "VIRTUAL";
		case Entity::BINARYNAME:
			return "BINARYNAME";
		case Entity::RELEASE:
			return "RELEASE";
		case Entity::COMPONENTNAME:
			return "COMPONENTNAME";
		case Entity::COMPONENT:
			return "COMPONENT";
		case Entity::OR:
			return "OR";
		case Entity::HAS_VERSION:
			return "HAS_VERSION";
		case Entity::CONTAINS:
			return "CONTAINS";
		case Entity::HAS_INSTANCE:
			return "HAS_INSTANCE";
		case Entity::DEPENDS:
			return "DEPENDS";
		case Entity::PRE_DEPENDS:
			return "PRE_DEPENDS";
		case Entity::CONFLICTS:
			return "CONFLICTS";
		case Entity::SUGGESTS:
			return "SUGGESTS";
		case Entity::RECOMMENDS:
			return "RECOMMENDS";
		case Entity::PROVIDED:
			return "PROVIDED";
		case Entity::ARCHITECTURE:
			return "ARCHITECTURE";
		case Entity::FOR_ARCHITECTURE:
			return "FOR_ARCHITECTURE";
		case Entity::INCLUDES_ARCH:
			return "INCLUDES_ARCH";
		default:
			return "INVALID";
	}
}

string Entity::toGraphviz() {
	return string("");
}
