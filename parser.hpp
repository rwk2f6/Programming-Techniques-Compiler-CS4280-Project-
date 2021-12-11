#ifndef PARSER_HPP
#define PARSER_HPP
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include "scanner.hpp"
#include "node.hpp"

//Scanner function is utilized to pull Tokens from the vector, not actual scanner
Token scanner();
//Parser is called in main, taking in the file, passing the file to the FSADriver, and then parser builds a tree with the vector FSADriver returns
Node * parser(std::string fileName);
//Non-terminal functions
Node * program();
Node * block();
Node * vars();
Node * expr();
Node * N();
Node * A();
Node * M();
Node * R();
Node * stats();
Node * mStat();
Node * stat();
Node * in();
Node * out();
Node * ifStatement();
Node * loop();
Node * assign();
Node * RO();
Node * label();
Node * gotoStatement();
//Error message function
void tkErrorMessage(Token, tokenID);
//Node functions
Node * newNode(nodeType);
void printNode(Node, int);
void preorderTraversal(Node *, int);

#endif