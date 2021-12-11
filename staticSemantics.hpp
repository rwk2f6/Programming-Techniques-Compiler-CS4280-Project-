#ifndef STATICSEMANTICS_HPP
#define STATICSEMANTICS_HPP
#include "node.hpp"
#include <string>
#include <vector>

void semanticsChecker(Node *);
void pushGlobalVars(Node *);
void pushLocalVars(Node *, int &);
int find(std::string);
int findLocalVars(std::string);
void treeTraversal(Node *);
void checkNodeVars(Node *);
void declarationErrorMessage(std::string);
void noDeclarationErrorMessage(std::string);

#endif