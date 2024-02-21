#include "LetObj.h"

static void concatString(char* dest, const char* src)
{
	int i = 0;
	int j = 0;

	while (dest[i] != '\0') { ++i; }
	while (src[j] != '\0') { dest[i] = src[j]; ++i, ++j; }
	dest[i] = '\0';
}

LetObj::LetObj()
{
	state = State::UNDEFINED;
	length = 0;
}

LetObj::LetObj(int val)
{
	state = State::INT;
	length = 0;
	integer = val;
}

LetObj::LetObj(bool val)
{
	state = State::BOOLEAN;
	length = 0;
	boolean = val;
}

LetObj::LetObj(const char* str)
{
	copyString(str);
}

LetObj::LetObj(const LetObj& rhs)
{
	copy(rhs);
}

LetObj::LetObj(const std::vector<LetObj>& newArr)
{
	state = State::ARRAY;
	arr = newArr;
	length = 0;
}

void LetObj::copy(const LetObj& rhs)
{
	state = rhs.state;
	length = rhs.length;

	switch (rhs.state)
	{
		case LetObj::State::BOOLEAN:
			boolean = rhs.boolean;
			break;
		case LetObj::State::INT:
			integer = rhs.integer;
			break;
		case LetObj::State::NULL_:
			break;
		case LetObj::State::UNDEFINED:
			break;
		case LetObj::State::STRING:
			copyString(rhs.buffer);
			break;
		case State::ARRAY:
			arr = rhs.arr;
			break;
		default:
			break;
	}
}

void LetObj::copyString(const char* newStr)
{
	int i = 0;
	while (newStr[i] != '\0') {
		buffer[i] = newStr[i];
		++i;
	}
	buffer[i] = '\0';
	length = i;
	state = State::STRING;
}

bool LetObj::operator==(const LetObj& rhs) const
{
	if (state == rhs.state) {
		switch (state)
		{
		case State::INT:
			return integer == rhs.integer;
		case State::BOOLEAN:
			return boolean == rhs.boolean;
		case State::STRING:
			return !std::strcmp(buffer, rhs.buffer);
		case State::ARRAY:
			return arr == rhs.arr;
		default:
			return true;
		}
	}

	if (IsNullOrUndefined(*this) || IsNullOrUndefined(rhs)) return false;

	if (state == State::STRING) {
		if (rhs.state == State::ARRAY) return false;

		return !std::strcmp(buffer, std::to_string(rhs.getVal()).c_str());
	}
	else if (rhs.state == State::STRING) {
		if (state == State::ARRAY) return false;

		return !std::strcmp(rhs.buffer, std::to_string(getVal()).c_str());
	}

	if ((state == State::ARRAY && rhs.state != State::ARRAY) || (state != State::ARRAY && rhs.state == State::ARRAY))
		return false;

	return getVal() == rhs.getVal();
}

bool LetObj::operator==(int val) const
{
	if (IsNullOrUndefined(*this) || state == State::ARRAY) return false;

	if (state == State::STRING) {
		std::string valStr = std::to_string(val);

		return !std::strcmp(valStr.c_str(), buffer);
	}

	return getVal() == val;
}

bool LetObj::operator==(const std::string& str)
{
	if (IsNullOrUndefined(*this) || state == State::ARRAY) return false;

	if (state == State::STRING) {
		return !std::strcmp(str.c_str(), buffer);
	}

	std::string valStr = std::to_string(getVal());

	return valStr == str;
}

bool LetObj::operator==(bool val) const
{
	if (IsNullOrUndefined(*this) || state == State::ARRAY) return false;
	if (state == State::STRING) return val;
	return getVal() == val;
}

bool LetObj::operator==(const std::vector<LetObj>& rhs) const
{
	if (state != State::ARRAY) return false;
	return arr == rhs;
}

LetObj& LetObj::operator=(int val)
{
	state = State::INT;
	integer = val;
	return *this;
}

LetObj& LetObj::operator=(bool val)
{
	state = State::BOOLEAN;
	boolean = val;
	return *this;
}

LetObj& LetObj::operator=(const char* newStr)
{
	copyString(newStr);
	return *this;
}

LetObj& LetObj::operator = (const LetObj& rhs)
{
	copy(rhs);
	return *this;
}

LetObj& LetObj::operator=(const State& newState)
{
	state = newState;
	return *this;
}

LetObj& LetObj::operator=(const std::vector<LetObj>& newArr)
{
	state = State::ARRAY;
	arr = newArr;
	return *this;
}

LetObj& LetObj::operator++()
{
	if (state == State::INT) {
		++integer;
	}
	else if (state == State::BOOLEAN) {
		boolean = !boolean;
	}

	return *this;
}

LetObj LetObj::operator++(int)
{
	LetObj clone(*this);
	operator++();
	return clone;
}

std::variant<LetObj, char> LetObj::operator[](int index)
{
	if (state == State::ARRAY) {
		return arr[index];
	}
	else if (state == State::STRING) {
		return buffer[index];
	}

	throw "Cannot use the index operator on a non iterable type!";
}

bool LetObj::IsNullOrUndefined(const LetObj& rhs)
{
	if (rhs.state == State::NULL_ || rhs.state == State::UNDEFINED)
		return true;
	return false;
}

std::ostream& operator<<(std::ostream& os, const LetObj& rhs)
{
	switch (rhs.state)
	{
		case LetObj::State::BOOLEAN:
			if (rhs.boolean) os << "true";
			else os << "false";
			break;
		case LetObj::State::INT:
			os << rhs.integer;
			break;
		case LetObj::State::NULL_:
			os << "null";
			break;
		case LetObj::State::UNDEFINED:
			os << "undefined";
			break;
		case LetObj::State::STRING:
			os << "'" << rhs.buffer << "'";
			break;
		case LetObj::State::ARRAY:
			os << "(" << rhs.arr.size() << ") [";
			{
				if (rhs.arr.size() == 0) {
					os << "]";
					break;
				}
				for (int i = 0; i < rhs.arr.size() - 1; ++i) {
					os << rhs.arr[i] << ", ";
				}
				if (rhs.arr.size() >= 1) {
					os << rhs.arr[rhs.arr.size() - 1];
				}
			}
			os << "]";
			break;
		default:
			break;
	}
	return os;
}

std::istream& operator>>(std::istream& in, LetObj& rhs)
{
	std::string line;
	in >> line;
	rhs.copyString(line.c_str());
	rhs.state = LetObj::State::STRING;
	return in;
}

LetObj operator+(const LetObj& lhs, const LetObj& rhs)
{
	if (LetObj::IsNullOrUndefined(lhs) || LetObj::IsNullOrUndefined(lhs))
		throw "Cannot add null or undefined values!";
	if (lhs.state == LetObj::State::ARRAY || rhs.state == LetObj::State::ARRAY)
		throw "Cannot add two arrays!";
	LetObj res;

	if(lhs.state != LetObj::State::STRING && rhs.state != LetObj::State::STRING) {
		res.state = LetObj::State::INT;
		res.integer = 0;
		
		if (lhs.state != LetObj::State::BOOLEAN) {
			res.integer += lhs.integer;
		}
		else {
			res.integer += lhs.integer;
		}
		if (rhs.state != LetObj::State::BOOLEAN) {
			res.integer += rhs.integer;
		}
		else {
			res.integer += rhs.integer;
		}
	}
	else if (lhs.state == LetObj::State::STRING && rhs.state == LetObj::State::STRING) {
		res.copyString(lhs.buffer);
		concatString(res.buffer, rhs.buffer);
		res.length = lhs.length + rhs.length;
	}
	else {
		res.state = LetObj::State::STRING;

		if (lhs.state == LetObj::State::STRING) {
			res.copyString(lhs.buffer);
			concatString(res.buffer, std::to_string(rhs.getVal()).c_str());
			res.length = lhs.length + std::to_string(rhs.getVal()).length();
		}
		else {
			res.copyString(std::to_string(lhs.getVal()).c_str());
			concatString(res.buffer, rhs.buffer);
			res.length = std::to_string(lhs.getVal()).length() + rhs.length;
		}
	}

	return res;
}

LetObj operator-(const LetObj& lhs, const LetObj& rhs)
{
	if (LetObj::IsNullOrUndefined(lhs) || LetObj::IsNullOrUndefined(lhs))
		throw "Cannot subtract null or undefined values!";
	if (lhs.state == LetObj::State::ARRAY || rhs.state == LetObj::State::ARRAY)
		throw "Cannot subtract two arrays!";
	
	LetObj res;
	res.state = LetObj::State::INT;

	int lhsVal = 0;
	int rhsVal = 0;

	if (lhs.state == LetObj::State::STRING) {
		try {
			lhsVal = std::stoi(lhs.buffer);
		}
		catch (...) {
			lhsVal = 0;
		}
	}
	else {
		lhsVal = lhs.getVal();
	}

	if (rhs.state == LetObj::State::STRING) {
		try {
			rhsVal = std::stoi(rhs.buffer);
		}
		catch (...) {
			rhsVal = 0;
		}
	}
	else {
		rhsVal = rhs.getVal();
	}

	res.integer = lhsVal - rhsVal;

	return res;
}

LetObj operator*(const LetObj& lhs, const LetObj& rhs)
{
	if (LetObj::IsNullOrUndefined(lhs) || LetObj::IsNullOrUndefined(lhs))
		throw "Cannot multiply null or undefined values!";
	if (lhs.state == LetObj::State::STRING && rhs.state == LetObj::State::STRING)
		throw "Cannot multiply two strings!";
	if (lhs.state == LetObj::State::ARRAY || rhs.state == LetObj::State::ARRAY)
		throw "Cannot multiply two arrays!";

	LetObj res;

	if (lhs.state == LetObj::State::STRING) {
		res = "";
		for (int i = 0; i < rhs.getVal(); ++i) {
			concatString(res.buffer, lhs.buffer);
		}

		res.length = lhs.length * rhs.getVal();
	}
	else if(rhs.state == LetObj::State::STRING){
		res = "";
		for (int i = 0; i < lhs.getVal(); ++i) {
			concatString(res.buffer, rhs.buffer);
		}

		res.length = rhs.length * lhs.getVal();
	}
	else {
		res = lhs.getVal() * rhs.getVal();
	}

	return res;
}

LetObj operator/(const LetObj& lhs, const LetObj& rhs)
{
	if (LetObj::IsNullOrUndefined(lhs) || LetObj::IsNullOrUndefined(lhs))
		throw "Cannot divide null or undefined values!";
	if (lhs.state == LetObj::State::STRING || rhs.state == LetObj::State::STRING)
		throw "Cannot divide string values!";
	if (lhs.state == LetObj::State::ARRAY || rhs.state == LetObj::State::ARRAY)
		throw "Cannot divide two arrays!";
	return LetObj(lhs.getVal() / rhs.getVal());
}

LetObj operator%(const LetObj& lhs, const LetObj& rhs)
{
	if (LetObj::IsNullOrUndefined(lhs) || LetObj::IsNullOrUndefined(lhs))
		throw "Cannot use module operator on null or undefined values!";
	if (lhs.state == LetObj::State::STRING || rhs.state == LetObj::State::STRING)
		throw "Cannot use module operator on strings!";
	if (lhs.state == LetObj::State::ARRAY || rhs.state == LetObj::State::ARRAY)
		throw "Cannot use module operator on two arrays!";

	return LetObj(lhs.getVal() % rhs.getVal());
}


std::ostream& operator << (std::ostream& os, const std::variant<LetObj, char>& var)
{
	if (var.index() == 0) {
		os << std::get<LetObj>(var);
	}
	else {
		os << std::get<char>(var);
	}

	return os;
}