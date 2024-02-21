#include "Lexer.h"

std::unordered_map<std::string, int> keywordsTable;

static bool IsAlphaString(const std::string& str)
{
	for (int i = 0; i < str.length(); ++i) {
		if (!std::isalpha(str[i])) {
			return false;
		}
	}

	return true;
}

static bool IsAlphaNumericString(const std::string& str)
{
	for (int i = 0; i < str.length(); ++i) {
		if (!std::isalnum(str[i])) {
			return false;
		}
	}
	return true;
}

static bool IsNumericString(const std::string& str)
{
	for (int i = 0; i < str.length(); ++i) {
		if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

static bool IsArithmeticOperator(const char& op)
{
	if (op == '+' || op == '-' || op == '/' || op == '*' || op == '%' || op == '=' || op == '|' || op == '&' || op == '^')
		return true;
	return false;
}

Token createToken(const std::string& str, int string_lit = 0)
{
	Token res;
	res.value = str;

	if (keywordsTable.find(str) != keywordsTable.end()) {
		res.type = (TokenType)keywordsTable[str];
		return res;
	}

	if (IsNumericString(str)) res.type = TokenType::INT_LITERAL;
	else if (IsAlphaString(str) || IsAlphaNumericString(str)) res.type = TokenType::IDENTIFIER;
	else if (IsArithmeticOperator(str[0])) res.type = TokenType::OPERATOR;
	else res.type = TokenType::IDENTIFIER;

	if (string_lit >= 2 && string_lit % 2 == 0) {
		res.type = TokenType::STR_LITERAL;
	}
}

Lexer::Lexer()
{
	keywordsTable["function"] = 0;
	keywordsTable["return"] = 1;
	keywordsTable[";"] = 2;
	keywordsTable["("] = 3;
	keywordsTable[")"] = 4;
	keywordsTable["{"] = 5;
	keywordsTable["}"] = 6;
	keywordsTable["identifier"] = 7;
	keywordsTable["int_literal"] = 8;
	keywordsTable["operator"] = 9;
	keywordsTable["["] = 10;
	keywordsTable["]"] = 11;
	keywordsTable["let"] = 12;
	//keywordsTable["\n"] = 13;
	keywordsTable["print"] = 14;
	keywordsTable[","] = 15;
	keywordsTable[":"] = 16;
	keywordsTable["endl"] = 17;
}

Lexer::~Lexer() {}

std::vector<Token> Lexer::tokenize(const std::string& input)
{
	std::vector<Token> tokens;
	std::string prev = "";
	int string_lit = 0;

	int cursor = 0;

	while (cursor < input.length())
	{
		if (keywordsTable.find(std::string() + (input[cursor])) != keywordsTable.end() && string_lit == 0) {
			if (prev != "") {
				tokens.push_back(createToken(prev, string_lit));
				prev = "";
			}
			tokens.push_back(createToken(std::string() + input[cursor]));
		}
		else if ((input[cursor] == ' ' || input[cursor] == '\n' || input[cursor] == '\t') && string_lit == 0) {
			if (prev != "") {
				tokens.push_back(createToken(prev, string_lit));
				prev = "";
			}
		}
		else if (input[cursor] == ' ' || input[cursor] == '\n' || input[cursor] == '\t' && string_lit) {
			prev += input[cursor];
		}
		else if (IsArithmeticOperator(input[cursor]) && string_lit == 0)
		{
			if (prev != "") {
				tokens.push_back(createToken(prev, string_lit));
				prev = "";
			}
			tokens.push_back(createToken(std::string() + input[cursor], string_lit));
		}
		else {

			prev += input[cursor];

			if (input[cursor] == '\"') {
				++string_lit;
			}
			if (string_lit >= 2) {
				tokens.push_back(createToken(prev, string_lit));
				prev = "";
				string_lit = 0;
			}
		}

		++cursor;
	}

	prevTokens = tokens;
	return tokens;
}

std::ostream& operator << (std::ostream& os, const Token& rhs)
{
	os << "<";

	switch (rhs.type) {
		case TokenType::FUNCTION:
			os << "function";
			break;
		case TokenType::IDENTIFIER:
			os << "identifier";
			break;
		case TokenType::LEFT_BRACE:
			os << "{";
			break;
		case TokenType::RIGHT_BRACE:
			os << "}";
			break;
		case TokenType::LEFT_P:
			os << "(";
			break;
		case TokenType::RIGHT_P:
			os << ")";
			break;
		case TokenType::SEMICOLON:
			os << ";";
			break;
		case TokenType::RETURN:
			os << "return";
			break;
		case TokenType::INT_LITERAL:
			os << "int_literal";
			break;
		case TokenType::OPERATOR:
			os << "operator";
			break;
		case TokenType::LEFT_BRACKET:
			os << "[";
			break;
		case TokenType::RIGHT_BRACKET:
			os << "]";
			break;
		case TokenType::LET:
			os << "type";
			break;
		case TokenType::NEWLINE:
			os << "\\n >";
			return os;
		case TokenType::PRINT:
			os << "print";
			break;
		case TokenType::COMMA:
			os << "COMMA";
			break;
		case TokenType::KEYEQUAL:
			os << ":=";
			break;
		case TokenType::ENDL:
			os << "ENDL";
			break;
		case TokenType::STR_LITERAL:
			os << "string_literal";
			break;
		default:
			os << "Invalid Token!";
			break;
	}
	
	os << " " << rhs.value << ">";
	return os;
}

bool Lexer::isValidToken(const std::string& name)
{
	return false;
}

void Lexer::printTokens() const
{
	for (int i = 0; i < prevTokens.size(); ++i) {
		std::cout << prevTokens[i] << "\n";
	}
}