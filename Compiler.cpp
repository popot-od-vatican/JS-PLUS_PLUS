#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <initializer_list>
#include "Parser.h"


int main(int argc, char** argv)
{
    std::fstream sourceFile("./source.pp", std::ios_base::in);
    std::fstream outputFile("./out.cpp", std::ios_base::out);

    std::stringstream contents;
    contents << sourceFile.rdbuf();

    Parser p;
    Lexer lexer;

    std::cout << contents.str();
    p.setTokens(lexer.tokenize(contents.str()));
    lexer.printTokens();

    p.dumpCode("./out.cpp");

    sourceFile.close();
    outputFile.close();

    return 0;
}
