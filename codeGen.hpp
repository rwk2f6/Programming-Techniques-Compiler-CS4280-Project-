#ifndef CODEGEN_HPP
#define CODEGEN_HPP
#include "node.hpp"
#include "token.hpp"
#include <string>
#include <vector>

std::string removeDotTXT(std::string &);
void genDriver(Node *, std::string &);
void kbGenDriver(Node *);
void genSetup(Node *, int);
void asmGen(Node *, int &);
void getGlobalVars(Node *);
void getLocalVars(Node *, int &);
void assignGenerator(Node *, int &);
void outGenerator(Node *, int &);
void exprGenerator(Node *, int &);
void nGenerator(Node *, int &);
void aGenerator(Node *, int &);
void mGenerator(Node *, int &);
void rGenerator(Node *, int &);
void inGenerator(Node *, int &);
void labelGenerator(Node *, int &);
void gotoGenerator(Node *, int &);
void loopGenerator(Node *, int &);
void ifGenerator(Node *, int &);
int checkLocal(std::string);

#endif