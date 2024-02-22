#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <initializer_list>
#include <exception>
#include "Parser.h"


int main(int argc, char** argv)
{
    LetObj x = 11;

    std::fstream sourceFile("./source.pp", std::ios_base::in);

    std::stringstream contents;
    contents << sourceFile.rdbuf();

    Parser p;
    Lexer lexer;

    std::cout << contents.str();
    p.setTokens(lexer.tokenize(contents.str()));
    lexer.printTokens();

    try {
        p.dumpCode("./out.cpp");
    }
    catch (std::exception& e) {
        std::cout << e.what();
    }
    catch (const char* msg) {
        std::cout << msg;
    }

    sourceFile.close();

    return 0;
}
