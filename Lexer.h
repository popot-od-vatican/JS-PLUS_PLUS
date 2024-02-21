#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

enum class TokenType
{
	FUNCTION,
	RETURN,
	SEMICOLON,
	LEFT_P,
	RIGHT_P,
	LEFT_BRACE,
	RIGHT_BRACE,
	IDENTIFIER,
	INT_LITERAL,
	OPERATOR,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LET,
	NEWLINE,
	PRINT,
	COMMA,
	KEYEQUAL,
	ENDL,
	STR_LITERAL,
};

struct Token
{
	TokenType type;
	std::string value;
	std::vector<std::string> params;

	Token() {}
};

class Lexer
{
	std::vector<Token> prevTokens;

	public:
		Lexer();
		~Lexer();

		std::vector<Token> tokenize(const std::string& input);

		void printTokens() const;

	private:

		bool isValidToken(const std::string& name);
};

