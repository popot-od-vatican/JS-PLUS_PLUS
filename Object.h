#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include "LetObj.h"

class Object
{
	std::unordered_map<std::string, LetObj> properties;

	public:
		Object();
		Object(const std::initializer_list< std::pair<std::string, std::string> >& list);

		LetObj& operator [] (const std::string& property);

		friend std::ostream& operator << (std::ostream& os, const Object& rhs);
};

