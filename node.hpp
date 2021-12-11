#ifndef NODE_HPP
#define NODE_HPP
#include "token.hpp"

//Enum to keep track of every possible node type in the tree
enum nodeType {PROGRAMnode, BLOCKnode, VARSnode, EXPRnode, Nnode, Anode, Mnode, Rnode, STATSnode, MSTATnode, STATnode, INnode, OUTnode, IFnode, LOOPnode, ASSIGNnode, ROnode, LABELnode, GOTOnode };

struct Node
{
    //Each node has a nodeType
    nodeType nodeT;
    //Tokens to keep track of Identifiers and Numbers present in a node
    Token tk1;
    Token tk2;
    //Children nodes are used when other non-terminal functions are called.
    //5 children are needed specifically for the ifstatement
    struct Node *childNode1;
    struct Node *childNode2;
    struct Node *childNode3;
    struct Node *childNode4;
    struct Node *childNode5;
};

#endif