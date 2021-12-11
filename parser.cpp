#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include "parser.hpp"

using namespace std;

//Vector that stores tokens from FSADriver
vector<Token> readTokens;
//Token to hold most recent token read from the vector
Token curTok;
//Token names and nodeNames for printing
string tokenNames2[41] = {" ", "Identifier", "Number", "Assignment Operator", "Greater Than", "Less Than", "Equal To", "Colon", "Colon Equal", "Plus", "Minus", "Multiplication", "Divison", "Mod", "Dot", "Left Parentheses", "Right Parentheses", "Comma", "L. Curly Bracket", "R. Curly Bracket", "SemiColon", "L. Square Bracket", "R. Square Bracket", "Keyword", "Start", "Stop", "Loop", "While", "For", "Label", "Exit", "Listen", "Talk", "Program", "If", "Then", "Assign", "Declare", "Jump", "Else", "EOF" };
string nodeTypes[] = {"Program", "Block", "Vars", "Expr", "N", "A", "M", "R", "Stats", "Mstat", "Stat", "In", "Out", "If", "Loop", "Assign", "Relational Operator", "Label", "Goto" };
int vectorIndex = 0;

//Function to iterate through vector and return a token
Token scanner()
{
    Token temptk = readTokens.at(vectorIndex);
    vectorIndex++;
    return temptk;
}

//Parser function called in main
Node * parser(string file)
{
    //Create root node
    Node *root;
    //Get vector of tokens from real scanner
    readTokens = FSADriver(file);
    //Store first token inside of curTok
    curTok = scanner();
    //Begin building tree
    root = program();

    if (curTok.tokenClass == EOF_tk)
    {
        cout << "Parser is complete" << endl;
    }
    return root;
}

/*
In all non-terminal functions, create a new node with the proper node type. All calls to other non-terminal functions are stored in the current node's
child nodes. If the non-terminal definition has tokens, make sure they are there, in the correct order. Otherwise, throw an error message specificying
which token was expected, and what was in it's place. If the non-terminal has an identifier or a number, store it in the node's two tokens
*/

//<program> -> <vars> program <block>
Node * program()
{
    Node *node = newNode(PROGRAMnode);
    //cout << "Program Non-terminal Called" << endl;
    node->childNode1 = vars();

    if (curTok.tokenClass == PROGRAM_tk)
    {
        curTok = scanner();
        node->childNode2 = block();
        return node;
    }
    else
    {
        tkErrorMessage(curTok, PROGRAM_tk);
        return NULL;
    }
}

//<block> -> start <vars> <stats> stop
Node * block()
{
    Node *node = newNode(BLOCKnode);
    //cout << "Block Non-terminal Called" << endl;
    
    if (curTok.tokenClass == START_tk)
    {
        curTok = scanner();
        node->childNode1 = vars();
        node->childNode2 = stats();

        if (curTok.tokenClass == STOP_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, STOP_tk);
        }
    }
    else
    {
        tkErrorMessage(curTok, START_tk);
    }
    return node;
}

//<vars> -> empty | declare Identifier = Integer ; <vars>
Node * vars()
{
    //cout << "Vars Non-terminal Called" << endl;
    Node *node = newNode(VARSnode);

    if (curTok.tokenClass == DECLARE_tk)
    {
        curTok = scanner();

        if (curTok.tokenClass == IDENT_tk)
        {
            node->tk1 = curTok;
            curTok = scanner();

            if (curTok.tokenClass == ASSOP_tk)
            {
                curTok = scanner();

                if (curTok.tokenClass == NUM_tk)
                {
                    node->tk2 = curTok;
                    curTok = scanner();

                    if (curTok.tokenClass == SEMIC_tk)
                    {
                        curTok = scanner();

                        node->childNode1 = vars();
                        return node;
                    }
                    else
                    {
                        tkErrorMessage(curTok, SEMIC_tk);
                        return NULL;
                    }
                }
                else
                {
                    tkErrorMessage(curTok, NUM_tk);
                    return NULL;
                }
            }
            else
            {
                tkErrorMessage(curTok, ASSOP_tk);
                return NULL;
            }
        }
    }
    else
    {
        return node;
    }
    return node;
}

//<expr> -> <N> + <expr> | <N>
Node * expr()
{
    //cout << "Expr Non-terminal Called" << endl;
    Node *node = newNode(EXPRnode);

    node->childNode1 = N();

    if (curTok.tokenClass == PLUS_tk)
    {
        node->tk1 = curTok;

        curTok = scanner();

        node->childNode2 = expr();

        return node;
    }
    else
    {
        return node;
    }
}

//<N> -> <A>/<N> | <A>*<N> | <A>
Node * N()
{
    //cout << "N Non-terminal Called" << endl;
    Node *node = newNode(Nnode);

    node->childNode1 = A();

    if (curTok.tokenClass == DIV_tk)
    {
        node->tk1 = curTok;

        curTok = scanner();

        node->childNode2 = N();
        return node;
    }
    else if (curTok.tokenClass == MULT_tk)
    {
        node->tk1 = curTok;

        curTok = scanner();

        node->childNode2 = N();
        return node;
    }
    else
    {
        return node;
    }
}

//<A> -> <M>-<A> | <M>
Node * A()
{
    //cout << "A Non-terminal Called" << endl;
    Node *node = newNode(Anode);

    node->childNode1 = M();

    if (curTok.tokenClass == MINUS_tk)
    {
        node->tk1 = curTok;

        curTok = scanner();

        node->childNode2 = A();
        return node;
    }
    else
    {
        return node;
    }
}

//<M> -> .<M> | <R>
Node * M()
{
    //cout << "M Non-terminal Called" << endl;
    Node *node = newNode(Mnode);

    if (curTok.tokenClass == PERIOD_tk)
    {
        node->tk1 = curTok;

        curTok = scanner();

        node->childNode1 = M();
        return node;
    }
    else
    {
        node->childNode1 = R();
        return node;
    }
}

//<R> -> ( <expr> ) | Identifier | Integer
Node * R()
{
    //cout << "R Non-terminal Called" << endl;
    Node *node = newNode(Rnode);

    if (curTok.tokenClass == LPARENTH_tk)
    {
        curTok = scanner();

        node->childNode1 = expr();

        if (curTok.tokenClass == RPARENTH_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, RPARENTH_tk);
            return node;
        }
    }
    else if (curTok.tokenClass == IDENT_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else if (curTok.tokenClass == NUM_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else
    {
        tkErrorMessage(curTok, LPARENTH_tk);
        return NULL;
    }
}

//<stats> -> <stat> <mStat>
Node * stats()
{
    //cout << "Stats Non-terminal Called" << endl;
    Node *node = newNode(STATSnode);

    node->childNode1 = stat();
    node->childNode2 = mStat();

    return node;
}

//<mStat> -> empty | <stat> <mStat>
Node * mStat()
{
    //cout << "mStat Non-terminal Called" << endl;
    Node *node = newNode(MSTATnode);

    if (curTok.tokenClass == LISTEN_tk || curTok.tokenClass == TALK_tk || curTok.tokenClass == IF_tk || curTok.tokenClass == WHILE_tk || curTok.tokenClass == ASSIGN_tk || curTok.tokenClass == JUMP_tk || curTok.tokenClass == LABEL_tk || curTok.tokenClass == START_tk)
    {
        node->childNode1 = stat();
        node->childNode2 = mStat();
        return node;
    }
    else
    {
        return NULL;
    }
    
}

//<stat> -> <in>; | <out>; | <block> | <if>; | <loop>; | <assign>; | <goto>; | <label>;
Node * stat()
{
    //cout << "Stat Non-terminal Called" << endl;
    Node *node = newNode(STATnode);

    if (curTok.tokenClass == LISTEN_tk)
    {
        curTok = scanner();
        node->childNode1 = in();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == TALK_tk)
    {
        curTok = scanner();
        node->childNode1 = out();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == START_tk)
    {
        node->childNode1 = block();
        return node;
    }
    else if (curTok.tokenClass == IF_tk)
    {
        curTok = scanner();
        node->childNode1 = ifStatement();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == WHILE_tk)
    {
        curTok = scanner();
        node->childNode1 = loop();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == ASSIGN_tk)
    {
        curTok = scanner();
        node->childNode1 = assign();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == JUMP_tk)
    {
        curTok = scanner();
        node->childNode1 = gotoStatement();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == LABEL_tk)
    {
        curTok = scanner();
        node->childNode1 = label();
        if (curTok.tokenClass == SEMIC_tk)
        {
            curTok = scanner();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, SEMIC_tk);
            return NULL;
        }
    }
    else
    {
        cout << "If you got here, you messed up" << endl;
        return NULL;
    }
    return NULL;
}

//<in> -> listen Identifier
Node * in()
{
    //cout << "In Non-terminal Called" << endl;
    Node *node = newNode(INnode);

    if(curTok.tokenClass == IDENT_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else
    {
        tkErrorMessage(curTok, IDENT_tk);
        return NULL;
    }

}

//<out> -> talk <expr>
Node * out()
{
    //cout << "Out Non-terminal Called" << endl;
    Node *node = newNode(OUTnode);

    node->childNode1 = expr();

    return node;
}

//<if> -> if [ <expr> <RO> <expr> ] then <stat> | if [ <expr> <RO> <expr> ] then <stat> else <stat>
Node * ifStatement()
{
    //cout << "If Non-terminal Called" << endl;
    Node *node = newNode(IFnode);

    if (curTok.tokenClass == LSBRACKET_tk)
    {
        curTok = scanner();

        node->childNode1 = expr();
        node->childNode2 = RO();
        node->childNode3 = expr();

        if (curTok.tokenClass == RSBRACKET_tk)
        {
            curTok = scanner();

            if (curTok.tokenClass == THEN_tk)
            {
                curTok = scanner();

                node->childNode4 = stat();

                if (curTok.tokenClass == ELSE_tk)
                {
                    curTok = scanner();

                    node->childNode5 = stat();
                    return node;
                }
                else
                {
                    return node;
                }
            }
        }
        else
        {
            tkErrorMessage(curTok, RSBRACKET_tk);
            return NULL;
        }
    }
    else
    {
        tkErrorMessage(curTok, LSBRACKET_tk);
        return NULL;
    }
    return node;
}

//<loop> -> while [ <expr> <RO> <expr> ] <stat>
Node * loop()
{
    //cout << "Loop Non-terminal Called" << endl;
    Node *node = newNode(LOOPnode);

    if (curTok.tokenClass == LSBRACKET_tk)
    {
        curTok = scanner();

        node->childNode1 = expr();
        node->childNode2 = RO();
        node->childNode3 = expr();

        if (curTok.tokenClass == RSBRACKET_tk)
        {
            curTok = scanner();

            node->childNode4 = stat();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, RSBRACKET_tk);
            return NULL;
        }
    }
    else
    {
        tkErrorMessage(curTok, LSBRACKET_tk);
        return NULL;
    }
}

//<assign> -> assign Identifier = <expr>
Node * assign()
{
    //cout << "Assign Non-terminal Called" << endl;
    Node *node = newNode(ASSIGNnode);

    if (curTok.tokenClass == IDENT_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();

        if (curTok.tokenClass == ASSOP_tk)
        {
            curTok = scanner();

            node->childNode1 = expr();
            return node;
        }
        else
        {
            tkErrorMessage(curTok, ASSOP_tk);
            return NULL;
        }
    }
    else
    {
        tkErrorMessage(curTok, IDENT_tk);
        return NULL;
    }
}

//<RO> -> > | < | == | { == } (three tokens) | %
Node * RO()
{
    //cout << "RO Non-terminal Called" << endl;
    Node *node = newNode(ROnode);

    if (curTok.tokenClass == GTHEN_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else if (curTok.tokenClass == LTHEN_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else if (curTok.tokenClass == EQUAL_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else if (curTok.tokenClass == LCBRACKET_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();

        if(curTok.tokenClass == EQUAL_tk)
        {
            //node->tk1 = curTok;
            curTok = scanner();

            if (curTok.tokenClass == RCBRACKET_tk)
            {
                curTok = scanner();
                return node;
            }
            else
            {
                tkErrorMessage(curTok, RCBRACKET_tk);
                return NULL;
            }
        }
        else
        {
            tkErrorMessage(curTok, EQUAL_tk);
            return NULL;
        }
    }
    else if (curTok.tokenClass == MOD_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else
    {
        tkErrorMessage(curTok, GTHEN_tk);
        return NULL;
    }
}

//<label> -> label Identifier
Node * label()
{
    //cout << "Label Non-terminal Called" << endl;
    Node *node = newNode(LABELnode);

    if (curTok.tokenClass == IDENT_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else
    {
        tkErrorMessage(curTok, IDENT_tk);
        return NULL;
    }
}

//<goto> -> jump Identifier
Node * gotoStatement()
{
    //cout << "Goto Non-terminal Called" << endl;
    Node *node = newNode(GOTOnode);

    if (curTok.tokenClass == IDENT_tk)
    {
        node->tk1 = curTok;
        curTok = scanner();
        return node;
    }
    else
    {
        tkErrorMessage(curTok, IDENT_tk);
        return NULL;
    }
}

//Create new node for tree, with the nodeType passed to it
Node * newNode(nodeType nodeT)
{
    Node* node = new Node();
    //Set node type
    node->nodeT = nodeT;

    //Initialize tokens as empty
    Token token1, token2;
    token1.tokenClass = WhiteSpace;
    token1.TokenInstance = "";
    token1.lineNum = 0;
    token2.tokenClass = WhiteSpace;
    token2.TokenInstance = "";
    token2.lineNum = 0;

    node->tk1 = token1;
    node->tk2 = token2;

    //Set all childNode pointers to NULL
    node->childNode1 = NULL;
    node->childNode2 = NULL;
    node->childNode3 = NULL;
    node->childNode4 = NULL;
    node->childNode5 = NULL;

    return node;
}

//Function to display Nodes, passing indent to keep track of Node depth
void printNode(Node *node, int indent)
{
    for(int i = 0; i < indent; i ++)
    {
        cout << "   ";
    }

    cout << "Node Type: " << nodeTypes[node->nodeT] << endl;

    //If the node has tokens, display them
    if(node->tk1.tokenClass != WhiteSpace)
    {
        for(int i = 0; i < indent; i ++)
        {
            cout << "   ";
        }

        cout << "Token1 { Class: " << tokenNames2[node->tk1.tokenClass] << ", Instance: " << node->tk1.TokenInstance << ", Line Number: " << node->tk1.lineNum << " }" << endl;
    }

    if(node->tk2.tokenClass != WhiteSpace)
    {
        for(int i = 0; i < indent; i ++)
        {
            cout << "   ";
        }

        cout << "Token2 { Class: " << tokenNames2[node->tk2.tokenClass] << ", Instance: " << node->tk2.TokenInstance << ", Line Number: " << node->tk2.lineNum << " }" << endl;
    }
}

//Preorder traversal of the tree generated to print nodes
void preorderTraversal(Node *rootNode, int indent)
{
    if (rootNode != NULL)
    {
        printNode(rootNode, indent);
        preorderTraversal(rootNode->childNode1, ++indent);
        preorderTraversal(rootNode->childNode2, ++indent);
        preorderTraversal(rootNode->childNode3, ++indent);
        preorderTraversal(rootNode->childNode4, ++indent);
        preorderTraversal(rootNode->childNode5, ++indent);
    }
}

//Error showing which token caused the error and which token was expected
void tkErrorMessage(Token curToken, tokenID tokenType)
{
    cout << "Error caused by token: " << curToken.TokenInstance << endl;
    cout << "Expected token: " << tokenNames2[tokenType] << endl;
    exit(0);
}