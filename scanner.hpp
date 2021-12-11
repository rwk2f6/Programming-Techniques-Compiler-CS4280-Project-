#ifndef SCANNER_HPP
#define SCANNER_HPP
#include "token.hpp"
#include <iostream>
#include <fstream>
#include <ctype.h>
#include <string>
#include <stdio.h>
#include <vector>

//Container for current line being read in file
struct strContainer
{
    std::string lineText = "";
    int lineNum;
    bool eof = false;
};

void errorMessage();
void openFile(std::string);
std::string filterComment(std::string);
bool whitespaceLine(std::string);
std::vector<Token> FSADriver(std::string);
void printTokens(vector<Token>);
strContainer readLine(std::ifstream &);
int findColumnNum(char);
bool checkForKeyword(std::string);
Token identifyKeyword(std::string);

#endif