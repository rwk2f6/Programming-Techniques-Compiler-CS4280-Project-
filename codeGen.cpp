#include "codeGen.hpp"
#include "staticSemantics.hpp"
#include <iostream>
#include <fstream>

using namespace std;

//Vectors for global and local variables
vector<string> globalVarsStack;
vector<string> localVarsStack;
ofstream file;
//Ints to keep track of temporary variables created in the code
int tempVarsCount = 0;
int tempLabelCount = 0;

string removeDotTXT(string & fileName)
{
    //Remove ".txt" from the input file
    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();
    fileName.pop_back();
    return fileName;
}

//Generation Driver is called from main with the root node. Use for file input
void genDriver(Node * root, string & fileName)
{
    removeDotTXT(fileName);
    fileName = fileName + ".asm";
    //Create/open file
    file.open(fileName);

    if (!file.is_open())
    {
        cout << "File didn't open! Exiting..." << endl;
        exit(0);
    }

    //Check if childNode1 has an global variable declarations
    getGlobalVars(root->childNode1);
    //Generation setup is called to check for Block nodes and other tokens that need code generation
    genSetup(root->childNode2, 0);

    //Once we reach end of file, write STOP and declare all variables used
    file << "STOP\n";

    for (int i = 0; i < globalVarsStack.size(); i++)
    {
        file << globalVarsStack.at(i) << " 0\n";
    }

    for (int i = 0; i < tempVarsCount; i++)
    {
        file << "T" << i << " 0\n";
    }

    file << "Temp 0\n";

    //Close the file
    file.close();
    cout << "Code Generation is complete!" << endl;
}

//Generation Driver is called from main with the root node. Use for keyboard input
void kbGenDriver(Node * root)
{
    //Create/open file
    file.open("kb.asm");

    if (!file.is_open())
    {
        cout << "File didn't open! Exiting..." << endl;
        exit(0);
    }

    //Check if childNode1 has an global variable declarations
    getGlobalVars(root->childNode1);
    //Generation setup is called to check for Block nodes and other tokens that need code generation
    genSetup(root->childNode2, 0);

    //Once we reach end of file, write STOP and declare all variables used
    file << "STOP\n";

    for (int i = 0; i < globalVarsStack.size(); i++)
    {
        file << globalVarsStack.at(i) << " 0\n";
    }

    for (int i = 0; i < tempVarsCount; i++)
    {
        file << "T" << i << " 0\n";
    }

    file << "Temp 0\n";

    //Close the file
    file.close();
    cout << "Code Generation is complete!" << endl;
}

//Checks if the node is null or a block. If it's a block, get localVars and setup generation for the block
//Else, generate the asm code for any given node Type
void genSetup(Node * node, int varsCount)
{
    if (node == NULL)
    {
        return;
    }

    if (node->nodeT == BLOCKnode)
    {
        int localVarsCount = 0;
        getLocalVars(node->childNode1, localVarsCount);
        genSetup(node->childNode2, localVarsCount);
        //After we leave a local scope, pop all vars from the stack
        for (int i = 0; i < localVarsCount; i++)
        {
            localVarsStack.pop_back();
        }
    }
    else
    {
        //Generate asm code call
        asmGen(node, varsCount);
    }
}

void asmGen(Node * node, int & varsCount)
{
    //If a child node is NULL, we just return
    if(node == NULL)
    {
        return;
    }

    //Check node type and call the proper generator function
    if(node->nodeT == ASSIGNnode)
    {
        assignGenerator(node, varsCount);
    }
    else if(node->nodeT == EXPRnode)
    {
        exprGenerator(node, varsCount);
    }
    else if(node->nodeT == Nnode)
    {
        nGenerator(node, varsCount);
    }
    else if(node->nodeT == Anode)
    {
        aGenerator(node, varsCount);
    }
    else if(node->nodeT == Mnode)
    {
        mGenerator(node, varsCount);
    }
    else if(node->nodeT == Rnode)
    {
        rGenerator(node, varsCount);
    }
    else if(node->nodeT == INnode)
    {
        inGenerator(node, varsCount);
    }
    else if(node->nodeT == OUTnode)
    {
        outGenerator(node, varsCount);
    }
    else if(node->nodeT == IFnode)
    {
        ifGenerator(node, varsCount);
    }
    else if(node->nodeT == LOOPnode)
    {
        loopGenerator(node, varsCount);
    }
    else if(node->nodeT == LABELnode)
    {
        labelGenerator(node, varsCount);
    }
    else if(node->nodeT == GOTOnode)
    {
        gotoGenerator(node, varsCount);
    }
    //If there isn't a node type that needs generating, check all child nodes
    else
    {
        asmGen(node->childNode1, varsCount);
        asmGen(node->childNode2, varsCount);
        asmGen(node->childNode3, varsCount);
        asmGen(node->childNode4, varsCount);
        asmGen(node->childNode5, varsCount);
    }
}

void getGlobalVars(Node * node)
{
    //Check if token is blank/NULL, otherwise push the token to the Global stack
    if (node->tk1.TokenInstance == "")
    {
        return;
    }
    else
    {
        globalVarsStack.push_back(node->tk1.TokenInstance);
        //ASM to store the variable inside of the variable from the global stack
        file << "LOAD " << node->tk2.TokenInstance << "\n";
        file << "STORE " << node->tk1.TokenInstance << "\n";
    }
    //getGlobal calls itself recursively, and will break when it tokenInstance is blank/NULL
    getGlobalVars(node->childNode1);
}

//Similar to getGlobalVars, but generates different ASM code
void getLocalVars(Node * node, int & locVarsCount)
{
    if (node->tk1.TokenInstance == "")
    {
        return;
    }
    else
    {
        localVarsStack.push_back(node->tk1.TokenInstance);
        //This ASM code pushes the variable to the stack inside of the assembly language
        file << "PUSH\n";
        file << "LOAD " << node->tk2.TokenInstance << "\n";
        file << "STACKW 0\n";
        locVarsCount++;
    }
    getLocalVars(node->childNode1, locVarsCount);
}

void assignGenerator(Node * node, int & varsCount)
{
    genSetup(node->childNode1, varsCount);
    //See if variable is inside of localStack, and return it's index
    int index = checkLocal(node->tk1.TokenInstance);
    if(index != -50)
    {
        //Write to ASM stack
        file << "STACKW " << to_string(index) << "\n";
    }
    else
    {
        //Store the value inside of a variable
        file << "STORE " << node->tk1.TokenInstance << "\n";
    }
}

//Writes node passed to it
void outGenerator(Node * node, int & varsCount)
{
    genSetup(node->childNode1, varsCount);
    //Use a temp variable to hold the value we are outputting
    file << "STORE Temp\nWRITE Temp\n";
}

void exprGenerator(Node * node, int & varsCount)
{
    //Check addition first, otherwise check the childNode
    if (node->tk1.tokenClass == PLUS_tk)
    {
        genSetup(node->childNode2, varsCount);
        //Temp variable are kept track of with tempVarsCount
        string temp = "T" + to_string(tempVarsCount++);
        //Store the value of the accumulator in temp
        file << "STORE " << temp << "\n";
        //Go down the left side, and then add temp to that value
        genSetup(node->childNode1, varsCount);
        file << "ADD " << temp << "\n";
    }
    else
    {
        genSetup(node->childNode1, varsCount);
    }
}

void nGenerator(Node * node, int & varsCount)
{
    //Check for multiplication or division
    if (node->tk1.tokenClass == MULT_tk || node->tk1.tokenClass == DIV_tk)
    {
        genSetup(node->childNode2, varsCount);
        //Store value of accumulator in T
        string temp = "T" + to_string(tempVarsCount++);
        file << "STORE " << temp << "\n";
        genSetup(node->childNode1, varsCount);
        //Check if the token was actually MULT or DIV, and output the correct ASM code
        if (node->tk1.tokenClass == MULT_tk)
        {
            file << "MULT " << temp << "\n";
        }
        else
        {
            file << "DIV " << temp << "\n";
        }
    }
    else
    {
        genSetup(node->childNode1, varsCount);
    }
}

void aGenerator(Node * node, int & varsCount)
{
    //Check for subtraction
    if (node->tk1.tokenClass == MINUS_tk)
    {
        genSetup(node->childNode2, varsCount);
        string temp = "T" + to_string(tempVarsCount++);
        //Store value of accumulator in temp, then subtract temp from the left child
        file << "STORE " << temp << "\n";
        genSetup(node->childNode1, varsCount);
        file << "SUB " << temp << "\n";
    }
    else
    {
        genSetup(node->childNode1, varsCount);
    }
}

void mGenerator(Node * node, int & varsCount)
{
    //M generator has the . operator, which inverses the arg. We can just write MULT -1 to the file
    genSetup(node->childNode1, varsCount);
    if (node->tk1.tokenClass == PERIOD_tk)
    {
        file << "MULT -1\n";
    }
}

void rGenerator(Node * node, int & varsCount)
{   
    //R generator can either be an Integer, so we load the number from the variable
    if(node->tk1.tokenClass == NUM_tk)
    {
        file << "LOAD " << node->tk1.TokenInstance << "\n";
    }
    //It can be an Identifier, which needs to be check to see if we store it globally or locally
    else if(node->tk1.tokenClass == IDENT_tk)
    {
        //Check if the token is inside localVarsStack
        int index = checkLocal(node->tk1.TokenInstance);
        if(index != -50)
        {
            //Read the local variable from the stack
            file << "STACKR " << to_string(index) << "\n";
        }
        else
        {
            //Load the variable's value
            file << "LOAD " << node->tk1.TokenInstance << "\n";
        }
    }
    else
    {
        genSetup(node->childNode1, varsCount);
    }
    
}

void inGenerator(Node * node, int & varsCount)
{
    //Reads input, first by checking if the variable is on the stack
    int temp = checkLocal(node->tk1.TokenInstance);
    if (temp == -50)
    {
        //If the variable is not on the localStack, READ it to the token name
        file << "READ " << node->tk1.TokenInstance << "\n";
    }
    else
    //READ to a temp variable that is then writtn to the ASM stack
    {
        file << "READ Temp\n";
        file << "LOAD Temp\nSTACKW " << to_string(temp) << "\n";
    }
}

void labelGenerator(Node * node, int & varsCount)
{
    //Creates a label to jump to on a BR
    file << node->tk1.TokenInstance << ": NOOP\n";
}

void gotoGenerator(Node * node, int & varsCount)
{
    //Creates a branch using the token passed to it
    file << "BR " << node->tk1.TokenInstance << "\n";
}

void loopGenerator(Node * node, int & varsCount)
{
    //loopStart and loopEnd are created using tempLabelCount, so it can handle any amount of loops
    string loopStart = "L" + to_string(tempLabelCount++);
    string loopEnd = "L" + to_string(tempLabelCount++);
    
    //Check instances where RO isn't %
    if (node->childNode2->tk1.tokenClass != MOD_tk)
    {
        //Write your loopStart to the file so you can BR back to it
        file << loopStart << ": NOOP\n";
        //Store right side of RO in TEMP
        genSetup(node->childNode3, varsCount);
        file << "STORE Temp\n";
        //Get left side of RO, then call Sub temp
        genSetup(node->childNode1, varsCount);
        file << "SUB Temp\n";

        //Check Relational Operator
        if (node->childNode2->tk1.tokenClass == EQUAL_tk)
        {
            //If R0 is ==, then the subtraction needs to result in a 0 to exit loop
            file << "BRPOS " << loopEnd << "\nBRNEG " << loopEnd << "\n";
        }
        else if (node->childNode2->tk1.tokenClass == GTHEN_tk)
        {
            //If R0 is >, then the subtraction needs to result in a Positive to exit loop
            file << "BRZNEG " << loopEnd << "\n";
        }
        else if (node->childNode2->tk1.tokenClass == LTHEN_tk)
        {
            //If R0 is <, then the subtraction needs to result in a Negative to exit loop
            file << "BRZPOS " << loopEnd << "\n";
        }
        else
        {
            //Else RO is { == }, which means !=. Anything besides a 0 will exit the loop
            file << "BRZERO " << loopEnd << "\n";
        }
        //Generate the statement that follows the loop
        genSetup(node->childNode4, varsCount);
        //Write BR loopStart to go back to the beginning if the condition wasn't met, otherwise it will go to the loopEnd
        file << "BR " << loopStart << "\n";
        file << loopEnd << ": NOOP\n";
    }
    else
    //RO was %, which compares the two numbers signs. We can mult the numbers and see if the result is negative, which means signs were different
    {
        //Write your loopStart to the file so you can BR back to it
        file << loopStart << ": NOOP\n";
        //Check the right child, and store it in temp
        genSetup(node->childNode3, varsCount);
        file << "STORE Temp\n";
        //Check the left child, and multiply it by temp
        genSetup(node->childNode1, varsCount);
        file << "MULT Temp\n";
        //BRNEG jumps to the end of the loop as the numbers had different signs
        file << "BRNEG " << loopEnd << "\n";
        //Generate the statement that followed the loop
        genSetup(node->childNode4, varsCount);
        //Write BR loopStart to jump to be beginning if the condition wasn't met, else loopEnd
        file << "BR " << loopStart << "\n";
        file << loopEnd << ": NOOP\n";
    }    
}

void ifGenerator(Node * node, int & varsCount)
{
    //exit1 and exit are created using tempLabelCount, so it can handle any amount of loops
    string exit1 = "L" + to_string(tempLabelCount++);
    string exit2 = "L" + to_string(tempLabelCount++);

    //Check instances where RO isn't %
    if (node->childNode2->tk1.tokenClass != MOD_tk)
    {
        //Get right child and store it in temp
        genSetup(node->childNode3, varsCount);
        file << "STORE Temp\n";
        //Get left child and subtract temp from it
        genSetup(node->childNode1, varsCount);
        file << "SUB Temp\n";

        //Check Relational Operator
        if (node->childNode2->tk1.tokenClass == EQUAL_tk)
        {
            //If R0 is ==, then the subtraction needs to result in a 0 to exit loop
            file << "BRPOS " << exit1 << "\nBRNEG " << exit1 << "\n";
        }
        else if (node->childNode2->tk1.tokenClass == GTHEN_tk)
        {
            //If R0 is >, then the subtraction needs to result in a Positive to exit loop
            file << "BRZNEG " << exit1 << "\n";
        }
        else if (node->childNode2->tk1.tokenClass == LTHEN_tk)
        {
            //If R0 is <, then the subtraction needs to result in a Negative to exit loop
            file << "BRZPOS " << exit1 << "\n";
        }
        else
        {
            //Else RO is { == }, which means !=. Anything besides a 0 will exit the loop
            file << "BRZERO " << exit1 << "\n";
        }
        //Generate the statement that follows the then
        genSetup(node->childNode4, varsCount);
        //Go back to exit2 if the condition wasn't met, otherwise continue to exit1
        file << "BR " << exit2 << "\n";
        file << exit1 << ": NOOP\n";
        //Generate the statement that follows the else
        genSetup(node->childNode5, varsCount);
        file << exit2 << ": NOOP\n";
    }
    else
    //RO was %, which compares the two numbers signs. We can mult the numbers and see if the result is negative, which means signs were different
    {
        //Check right child and store it in temp
        genSetup(node->childNode3, varsCount);
        file << "STORE Temp\n";
        //Check left child and multiply it by temp
        genSetup(node->childNode1, varsCount);
        file << "MULT Temp\n";
        //If the result is negative, then the numbers had different signs and the RO was false
        file << "BRNEG " << exit1 << "\n";
        //Generate the statement that follows the then
        genSetup(node->childNode4, varsCount);
        file << "BR " << exit2 << "\n";
        file << exit1 << ": NOOP\n";
        //Generate the statement that follows the else
        genSetup(node->childNode5, varsCount);
        file << exit2 << ": NOOP\n";
    }    
}

int checkLocal(string varsName)
{
    //Checks the localVarsStack for the variable name passed to it
    for (int x = localVarsStack.size() - 1; x >= 0; x--)
    {
        if (localVarsStack.at(x) == varsName)
        {
            return x;
        }
    }
    //Arbitray number that shouldn't be returned as an actual index
    return -50;
}