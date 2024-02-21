#pragma once
#include <fstream>
#include <stack>
#include "Lexer.h"
#include "LetObj.h"
#include "Object.h"

class Parser
{
	std::vector<Token> tokens;

	public:
		Parser();

		bool Parse();
		void setTokens(std::vector<Token> newTokens);
		void dumpCode(const std::string& fileName);
};

