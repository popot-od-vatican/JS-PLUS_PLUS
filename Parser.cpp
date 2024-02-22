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

	std::unordered_map<std::string, std::unordered_map<std::string, bool> > declared_identifiers;
	std::unordered_map<std::string, std::string> functionBody;

	std::string functions = "";
	std::string outer = "";
	std::stack<char> squareBracketsStack;
	std::stack<char> curlyBracketsStack;
	std::stack<char> functionBodyBrackets;
	std::stack<char> parenthesesStack;
	std::stack <std::string> functionsStack;
	int lambdaCount = 0;

	for (int i = 0; i < tokens.size(); ++i) {
		std::string line = "";
		switch (tokens[i].type) {
			case TokenType::FUNCTION:
			{
				std::string functionName = "";
				if (!hasToken(i + 1) || peek(i + 1).type != TokenType::IDENTIFIER)
					throw "Function doesn't have a name!";
				functionName = peek(i + 1).value;
				declared_identifiers[functionName] = { {functionName, true} };
				bool closedArguments = false;
				// get arguments
				if (!hasToken(i + 2) || peek(i + 2).type != TokenType::LEFT_P)
					throw "Function doesn't have an opening ( for arguments!";

				std::string arguments = " (";
				i = i + 3;

				while (i < tokens.size() && tokens[i].type != TokenType::RIGHT_P) {
					if (tokens[i].type != TokenType::IDENTIFIER) {
						throw std::string("Expected an identifier!");
					}
					if (declared_identifiers[functionName].find(tokens[i].value) != declared_identifiers[functionName].end()) {
						throw "Redefinition of an  already defined argument\nIn function " + functionName;
					}
					declared_identifiers[functionName][tokens[i].value] = true;

					arguments += "LetObj " + tokens[i].value;

					if (hasToken(i + 1) && peek(i + 1).type == TokenType::COMMA) {
						arguments += ", ";
						++i;
					}
					++i;
				}

				if (i < tokens.size() && tokens[i].type == TokenType::RIGHT_P && hasToken(i - 1) && peek(i - 1).type == TokenType::COMMA)
					throw "Expected an identifer before closing )\nRemove extra ,";

				if (i < tokens.size() && tokens[i].type == TokenType::RIGHT_P) {
					closedArguments = true;
					arguments += ")";
				}
				else
					throw "Missing closing ) after arguments!";

				if (!hasToken(i + 1) || peek(i + 1).type != TokenType::LEFT_BRACE) {
					throw "Function " + functionName + " is mising it's body {}";
				}

				++i;

				line = "std::optional<LetObj> " + functionName + arguments + " {";

				functionBodyBrackets.push('{');

				functionBody[functionName] = line;

				if (functionsStack.size() == 0) {
					functionsStack.push(functionName);
				}
				else {
					// FUNKCIJA VO FUNKCIJA
					// PRAVI LAMBDA ILI NAJGORE DEFINIRAJ JA FUNKCIJATA
				}
				break;
			}
			case TokenType::RIGHT_BRACE:
			{
				if (functionsStack.size() != 0) {
					functionBody[functionsStack.top()] += "\n}\n";
					if (curlyBracketsStack.size() != 0)
						curlyBracketsStack.pop();
					else{
						functionsStack.pop();
						functionBodyBrackets.pop();
					}
				}
				else {
					if (curlyBracketsStack.empty()) {
						throw "Expected a opening { before a closing }!";
					}
					curlyBracketsStack.pop();
					line = "}\n";
					functionBody["main"] += line;
				}
				break;
			}
			
			case TokenType::LEFT_BRACE:
			{
				curlyBracketsStack.push('{');

				if (functionsStack.size() != 0) {
					functionBody[functionsStack.top()] += "{\n";
				}
				else {
					functionBody["main"] += "{\n";
				}
				break;
			}
			
			case TokenType::SEMICOLON:
			{
				if (!functionsStack.empty()) {
					functionBody[functionsStack.top()] += ";\n";
				}
				else {
					functionBody["main"] += ";\n";
				}
				break;
			}

			case TokenType::LET:
			{
				if (!hasToken(i + 1) || peek(i + 1).type != TokenType::IDENTIFIER)
					throw "let can only be used before an identifier!";

				line = "LetObj " + tokens[++i].value;
				std::string functionName = "";

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				if (declared_identifiers[functionName].find(tokens[i].value) != declared_identifiers[functionName].end())
					throw "Redefinition of the variable: " + tokens[i].value + " in function " + functionName;

				functionBody[functionName] += line;
				declared_identifiers[functionName][tokens[i].value] = true;

				break;
			}

			case TokenType::OPERATOR:
			{
				std::string functionName = "";

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				functionBody[functionName] += tokens[i].value;

				break;
			}

			case TokenType::INT_LITERAL:
			{
				std::string functionName = "";

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				functionBody[functionName] += tokens[i].value;

				break;
			}

			case TokenType::STR_LITERAL:
			{
				std::string functionName = "";

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				functionBody[functionName] += tokens[i].value;

				break;
			}

			case TokenType::IDENTIFIER:
			{
				std::string functionName = "";

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				if (declared_identifiers[functionName].find(tokens[i].value) == declared_identifiers[functionName].end()) {
					throw "In function " + functionName + " using an undeclared identifier: " + tokens[i].value;
				}

				functionBody[functionName] += tokens[i].value;

				break;
			}
			
			case TokenType::LEFT_P:
			{
				std::string functionName;

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}
				parenthesesStack.push('(');
				
				functionBody[functionName] += "(";

				break;
			}

			case TokenType::RIGHT_P:
			{
				if (parenthesesStack.empty()) {
					throw "Missing an opening )!";
				}

				parenthesesStack.pop();
				std::string functionName;

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				functionBody[functionName] += ")";

				break;
			}

			case TokenType::COMMA:
			{
				std::string functionName;

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				if (hasToken(i + 1) && (peek(i + 1).type == TokenType::COMMA))
					throw "Cannot use two commas next to each other";
				
				functionBody[functionName] += ", ";
				break;
			}

			case TokenType::ENDL:
			{
				std::string functionName;

				if (!functionsStack.empty()) {
					functionName = functionsStack.top();
				}
				else {
					functionName = "main";
				}

				functionBody[functionName] += "std::endl";
				break;
			}

			case TokenType::PRINT:
			{
				std::string functionName;

				if (!functionsStack.empty()) { functionName = functionsStack.top(); }
				else { functionName = "main"; }

				line = "std::cout << ";
				bool hasEnded = false;
				++i;

				while (hasToken(i)) {
					if (tokens[i].type != TokenType::IDENTIFIER && tokens[i].type != TokenType::INT_LITERAL && tokens[i].type != TokenType::STR_LITERAL
						&& tokens[i].type != TokenType::COMMA && tokens[i].type != TokenType::SEMICOLON && tokens[i].type != TokenType::ENDL &&
						tokens[i].type != TokenType::OPERATOR) {
						throw "Invalid use of the print statement!";
					}

					if (tokens[i].type == TokenType::SEMICOLON) {
						hasEnded = true;
						line += ";\n";
						break;
					}

					if (tokens[i].type == TokenType::COMMA) {
						line += " << ";
					}
					else
						line += tokens[i].value;

					++i;
				}

				if (!hasEnded)
					throw "Missing ; after print statement!";

				functionBody[functionName] += line;

				break;
			}

			case TokenType::FOR:
			{
				std::string functionName;

				if (!functionsStack.empty()) { functionName = functionsStack.top(); }
				else { functionName = "main"; }

				bool isClosed = false;

				if (!hasToken(i + 1) || peek(i + 1).type != TokenType::LEFT_P) {
					throw "Expected an opening ( after for!";
				}

				++i;

				while (hasToken(i)) {
					line += tokens[i].value;
					if (tokens[i].type == TokenType::RIGHT_P) {
						isClosed = true;
						break;
					}
					if (tokens[i].type == TokenType::LET) {
						line += " ";
					}
					else if (tokens[i].type == TokenType::SEMICOLON) {
						line += " ";
					}
					++i;
				}

				if (!isClosed)
					throw "Expected a closing )!";

				functionBody[functionName] += line;

				break;
			}
			
			case TokenType::IF:
			{
				std::string functionName;

				if (!functionsStack.empty()) { functionName = functionsStack.top(); }
				else { functionName = "main"; }

				functionBody[functionName] += "if ";

				break;
			}

			case TokenType::ELSE:
			{
				std::string functionName;

				if (!functionsStack.empty()) { functionName = functionsStack.top(); }
				else { functionName = "main"; }

				functionBody[functionName] += "else ";

				break;
			}

			case TokenType::COLON:
			{
				std::string functionName;

				if (!functionsStack.empty()) { functionName = functionsStack.top(); }
				else { functionName = "main"; }
				functionBody[functionName] += " : ";
				break;
			}
		}
	}

	std::cout << "Test:\n" << functionBody["test"] << "\n";
	std::cout << "Main:\n" << functionBody["main"] << "\n";

	if (squareBracketsStack.size() != 0) {
		std::cerr << "Missing square brackets []" << std::endl;
		return;
	}

	if (functionBodyBrackets.size() != 0) {
		std::cerr << "Function is missing {} brackets!" << std::endl;
		return;
	}

	if (!parenthesesStack.empty()) {
		std::cerr << "Missing () parentheses!" << std::endl;
		return;
	}
	
	if (curlyBracketsStack.size() != 0) {
		std::cerr << "Missing curly brackets {}" << std::endl;
		if (functionsStack.size() != 0) {
			std::cerr << "Function: " << functionsStack.top() << " is missing a closing }!\n";
		}
		return;
	}

	std::string headers = "#include <iostream>\n#include <vector>\n#include <string>\n#include <unordered_map>\n#include \"LetObj.h\"\n#include \"Object.h\"\n\n\n";

	for (const auto& x : functionBody) {
		functions += x.second;
	}

	outputCode += headers;
	outputCode += functions;
	outputCode += "\nint main() {\n";
	outputCode += functionBody["Main"];
	outputCode += "\nreturn 0;\n}";

	output << outputCode;

	std::cout << "========================\n";
	std::cout << "Compiled code:\n";
	std::cout << outputCode;
	std::cout << "\n========================";

	output.close();
}

bool Parser::hasToken(int pos)
{
	if (pos < 0 || pos >= tokens.size())
		return false;
	return true;
}

Token Parser::peek(int pos)
{
	return tokens[pos];
}
