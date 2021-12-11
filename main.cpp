#include <iostream>
#include <string>
#include "parser.hpp"
#include "staticSemantics.hpp"
#include "codeGen.hpp"

using namespace std;

int main(int argc, char* argv[]) 
{
	//Create string to store file text, and a rootNode for the tree
	Node * rootNode;
	string file = "";

	//argc == 2 means they included 1 argument, which is likely a file.
	if (argc == 2) 
	{
		file = argv[1];
		rootNode = parser(file);
		//preorderTraversal(rootNode, 0);
		semanticsChecker(rootNode);
		genDriver(rootNode, file);
	}
	//Too many arguments include
	else if (argc > 2)
	{
		cout << "Too many args!" << endl;
		return 0;
	}
	//No parameters provided
	else
	{
		cout << "No input provided! Enter text to run through the scanner! Press Ctrl + D when finished" << endl;
		rootNode = parser(file);
		//preorderTraversal(rootNode, 0);
		semanticsChecker(rootNode);
		kbGenDriver(rootNode);
	}
	
	return 0;
}