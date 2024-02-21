#include "Object.h"

Object::Object()
{

}

Object::Object(const std::initializer_list< std::pair<std::string, std::string> >& list)
{
	for (const auto& x : list) {
		properties[x.first] = LetObj(x.second.c_str());
	}
}

LetObj& Object::operator[](const std::string& property)
{
	if (properties.find(property) == properties.end()) {
		properties[property] = LetObj::State::UNDEFINED;
	}
	return properties[property];
}

std::ostream& operator<<(std::ostream& os, const Object& rhs)
{
	for (const auto& x : rhs.properties) {
		os << "{ " << x.first << " : " << x.second << " }\n";
	}
	return os;
}
