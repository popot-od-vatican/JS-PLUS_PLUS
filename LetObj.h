#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <variant>

class LetObj
{
public:
	enum class State
	{
		NULL_ = - 2,
		UNDEFINED = -1,
		INT = 0,
		BOOLEAN = 1,
		STRING = 2,
		ARRAY = 3,
	};
private:

	union
	{
		int integer;
		bool boolean;
		char buffer[512];
	};

	std::vector<LetObj> arr;

	int getVal() const
	{
		switch (state)
		{
			case State::INT:
				return integer;
			case State::BOOLEAN:
				return boolean;
			default:
				return -1;
		}
	}

	State state;
	int length;

	void copyString(const char*);
	void copy(const LetObj& rhs);

	public:
		LetObj();
		LetObj(int);
		LetObj(bool);
		LetObj(const char* str);
		LetObj(const LetObj& rhs);
		LetObj(const std::vector<LetObj>& newArr);

		bool operator == (const LetObj& rhs) const;
		bool operator == (int) const;
		bool operator == (const std::string& str);
		bool operator == (bool) const;
		bool operator == (const std::vector<LetObj>&) const;

		LetObj& operator = (int);
		LetObj& operator = (bool);
		LetObj& operator = (const char*);
		LetObj& operator = (const LetObj& rhs);
		LetObj& operator = (const State& newState);
		LetObj& operator = (const std::vector<LetObj>& newArr);

		LetObj& operator ++ ();
		LetObj operator ++ (int);

		std::variant<LetObj, char> operator [] (int index);

		friend std::ostream& operator << (std::ostream& os, const LetObj& rhs);
		friend std::istream& operator >> (std::istream& in, LetObj& rhs);
		friend LetObj operator + (const LetObj& lhs, const LetObj& rhs);
		friend LetObj operator - (const LetObj& lhs, const LetObj& rhs);
		friend LetObj operator * (const LetObj& lhs, const LetObj& rhs);
		friend LetObj operator / (const LetObj& lhs, const LetObj& rhs);
		friend LetObj operator % (const LetObj& lhs, const LetObj& rhs);

		static bool IsNullOrUndefined(const LetObj& rhs);
};


std::ostream& operator << (std::ostream& os, const std::variant<LetObj, char>& var);