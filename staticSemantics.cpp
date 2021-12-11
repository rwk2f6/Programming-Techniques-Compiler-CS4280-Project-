#include "staticSemantics.hpp"
#include <iostream>

using namespace std;

//Declare global variable count to 0
int varsCount = 0;
//Vector of strings to hold token instances
vector<string> variableStack;

//Function called in main to begin pushing variables to stack
void semanticsChecker(Node * root)
{
    pushGlobalVars(root->childNode1);
    checkNodeVars(root->childNode2);
    cout << "Static Semantics is complete!" << endl;
}

//Checks entire stack for variables
int find(string varsName)
{
    //Check global stack for variable
    for (int x = variableStack.size() - 1; x >= 0; x--)
    {
        if (variableStack.at(x) == varsName)
        {
            //cout << varsName << " found in variableStack" << endl;
            return x;
        }
    }
    //Arbitray number that shouldn't be returned as an actual index
    return -50;
}

int findLocalVars(string varsName)
{
    //Check local stack for variable, only checking up to the variable count
    for (int x = variableStack.size() - 1; x > varsCount - 1; x--)
    {
        if (variableStack.at(x) == varsName)
        {
            //cout << varsName << " found in variableStack" << endl;
            return x;
        }
    }
    //Arbitray number that shouldn't be returned as an actual index
    return -50;
}

//Push variables declared inside of a block/local scope
void pushLocalVars(Node * node, int &localVarsC)
{
    //If the token isn't an Identifier token, it doesn't need to be pushed to the stack
    if (node->tk1.tokenClass != IDENT_tk)
    {
        return;
    }

    //Check if variable has been declared in local scope before
    int checkStack = findLocalVars(node->tk1.TokenInstance);

    //If variable wasn't already in the stack, increment global variable count, push tokenInstance to the stack
    if (checkStack == -50)
    {
        localVarsC++;
        variableStack.push_back(node->tk1.TokenInstance);
        //cout << node->tk1.TokenInstance << " was pushed to the local stack" << endl;
    }
    //The variable was already declared, throw a double declaration error message
    else
    {
        //cout << "PushLocalVars called" << endl;
        declarationErrorMessage(node->tk1.TokenInstance);
    }
    
    //Make sure the childNode isn't null, otherwise push local variables from that child.
    if(node->childNode1 != NULL)
    {
        pushLocalVars(node->childNode1, localVarsC);
    }
    return;
}

//Push variables declared before program
void pushGlobalVars(Node * node)
{
    //If the token isn't an Identifier token, it doesn't need to be pushed to the stack
    if (node->tk1.tokenClass != IDENT_tk)
    {
        return;
    }

    //Check if variable has been declared before
    int checkStack = find(node->tk1.TokenInstance);

    //If variable wasn't already in the stack, increment global variable count, push tokenInstance to the stack
    if (checkStack == -50)
    {
        varsCount++;
        variableStack.push_back(node->tk1.TokenInstance);
        //cout << node->tk1.TokenInstance << " was pushed to the global stack" << endl;
    }
    //The variable was already declared, throw a double declaration error message
    else
    {
        //cout << "PushGlobalVars called" << endl;
        declarationErrorMessage(node->tk1.TokenInstance);
    }
    
    //Make sure the childNode isn't null, otherwise push global variables from that child.
    if(node->childNode1 != NULL)
    {
        pushGlobalVars(node->childNode1);
    }
    return;
}

//Traverse tree looking for variables
void treeTraversal(Node * node)
{
    //If node type is a Block node, it will have a local scope, so we visit Node
    if (node->nodeT == BLOCKnode)
    {
        checkNodeVars(node);
    }
    else
    {
        //Node could potential have 2 tokens, so we check them both. If either returns -50, the variable wasn't declared and we throw an error message and exit
        if(node->tk1.tokenClass == IDENT_tk)
        {
            if (find(node->tk1.TokenInstance) == -50)
            {
                noDeclarationErrorMessage(node->tk1.TokenInstance);
            }
        }
        if(node->tk2.tokenClass == IDENT_tk)
        {
            if (find(node->tk2.TokenInstance) == -50)
            {
                noDeclarationErrorMessage(node->tk2.TokenInstance);
            }
        }

        //Check to see if the current node's child nodes are NULL. If they aren't begin traversing inside of the child node.
        if(node->childNode1 != NULL)
        {
            treeTraversal(node->childNode1);
        }
        if(node->childNode2 != NULL)
        {
            treeTraversal(node->childNode2);
        }
        if(node->childNode3 != NULL)
        {
            treeTraversal(node->childNode3);
        }
        if(node->childNode4 != NULL)
        {
            treeTraversal(node->childNode4);
        }
        if(node->childNode5 != NULL)
        {
            treeTraversal(node->childNode5);
        }

    }
    
}

//Check node is called to check a block Node for local variables
void checkNodeVars(Node * node)
{
    int localVarsCount = 0;
    pushLocalVars(node->childNode1, localVarsCount);
    if (node->childNode2 != NULL)
    {
        treeTraversal(node->childNode2);
    }
    //Once you are done with current local scope, pop the variables from the stack
    for(int i = 0; i < localVarsCount; i++)
    {
        variableStack.pop_back();
    }
}

//Error if a token name was declared twice
void declarationErrorMessage(string tokenName)
{
    cout << "Error: Double declaration of " << tokenName << " token, exiting program..." << endl;
    exit(0);
}

//Error if a token wasn't declared at all
void noDeclarationErrorMessage(string tokenName)
{
    cout << "Error: Variable " << tokenName << " wasn't declared before! Exiting program..." << endl;
    exit(0);
}
