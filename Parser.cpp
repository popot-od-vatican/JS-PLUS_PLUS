#include "Parser.h"

Parser::Parser()
{

}

bool Parser::Parse()
{
	return true;
}

void Parser::setTokens(std::vector<Token> newTokens)
{
	tokens = std::move(newTokens);
}

void Parser::dumpCode(const std::string& fileName)
{
	std::fstream output(fileName, std::ios_base::out);
	std::string outputCode = "";

	int fnCount = 0;
	int ptCount = 0;
	int clCount = 0;
	int tbCount = 0;
	int objCount = 0;
	int printCount = 0;
	std::unordered_map<std::string, bool> declared_identifiers;

	std::string functions = "";
	std::string outer = "";
	std::stack<char> squareBracketsStack;
	std::stack<char> curlyBracketsStack;
	std::stack <std::string> functionsStack;
	bool skipArrayInit = false;

	for (int i = 0; i < tokens.size(); ++i) {
		std::string line = "";
		switch (tokens[i].type) {
			case TokenType::FUNCTION:
				line = "LetObj ";
				functionsStack.push(tokens[i].value);
				++fnCount;
				break;
			case TokenType::IDENTIFIER:
				if (i - 1 >= 0 && tokens[i - 1].type == TokenType::LEFT_P && declared_identifiers.find(tokens[i].value) == declared_identifiers.end()) {
					line = "LetObj ";
				}
				if (i - 1 >= 0 && tokens[i - 1].type == TokenType::COMMA && i - 2 >= 0 && tokens[i-2].type == TokenType::IDENTIFIER
					&& declared_identifiers.find(tokens[i].value) == declared_identifiers.end()) {
					line = "LetObj ";
				}
				line += tokens[i].value;
				declared_identifiers.insert({ tokens[i].value, true });
				break;
			case TokenType::INT_LITERAL:
				if (squareBracketsStack.size() >= 1 && !skipArrayInit) {
					line = "LetObj(" + tokens[i].value + ")";
					break;
				}
				line = tokens[i].value;
				break;
			case TokenType::LET:
				line = "LetObj ";
				break;
			case TokenType::OPERATOR:
				line = tokens[i].value;
				break;
			case TokenType::RETURN:
				line = "return ";
				break;
			case TokenType::SEMICOLON:
				if (printCount == 1) --printCount;
				line = ";\n";
				break;
			case TokenType::LEFT_P:
				line = "(";
				++ptCount;
				break;
			case TokenType::RIGHT_P:
				line = ")";
				--ptCount;
				break;
			case TokenType::LEFT_BRACE:
				curlyBracketsStack.push('{');
				line = "{\n";
				++tbCount;
				++clCount;
				break;
			case TokenType::RIGHT_BRACE:
				if (curlyBracketsStack.size() == 0) {
					std::cerr << "Missing curly bracket {" << std::endl;
					return;
				}
				curlyBracketsStack.pop();
				line = "}\n";
				--clCount;
				--tbCount;
				if (clCount == 0) {
					--fnCount;
					functions += line;
				}
				if (fnCount == 0) line += "\n";
				break;
			case TokenType::COMMA:
				if (printCount == 1) {
					line = " << ";
					break;
				}
				line = ", ";
				break;
			case TokenType::PRINT:
				printCount = 1;
				line = "std::cout << ";
				break;
			case TokenType::ENDL:
				line = "std::endl";
				break;
			case TokenType::STR_LITERAL:
				if (squareBracketsStack.size() >= 1 && !skipArrayInit) {
					line = "LetObj(" + tokens[i].value + ")";
					break;
				}
				line = tokens[i].value;
				break;
			case TokenType::LEFT_BRACKET:
				squareBracketsStack.push('[');
				if (i - 1 >= 0 && tokens[i - 1].type == TokenType::IDENTIFIER) {
					line = "[";
					skipArrayInit = true;
					break;
				}
				line = "std::vector<LetObj>({";
				break;
			case TokenType::RIGHT_BRACKET:
				if (squareBracketsStack.size() <= 0) {
					std::cerr << "Missing square bracket [" << std::endl;
					return;
				}
				squareBracketsStack.pop();
				if (skipArrayInit)
					line = "]";
				else
					line = "})";
				if (squareBracketsStack.size() == 0) skipArrayInit = false;
				break;
			default:
				break;
		}

		if (fnCount >= 1) functions += line;
		else if(fnCount == 0 && tokens[i].value != "}") outer += line;
	}

	if (squareBracketsStack.size() != 0) {
		std::cerr << "Missing square brackets []" << std::endl;
		return;
	}
	
	if (curlyBracketsStack.size() != 0) {
		std::cerr << "Missing curly brackets {}" << std::endl;
		return;
	}

	std::string headers = "#include <iostream>\n#include <vector>\n#include <string>\n#include <unordered_map>\n#include \"LetObj.h\"\n#include \"Object.h\"\n\n\n";

	outputCode += headers;
	outputCode += functions;
	outputCode += "\nint main() {\n";
	outputCode += outer;
	outputCode += "\nreturn 0;\n}";

	output << outputCode;

	output.close();
}
