#include "scanner.hpp"

using namespace std;

//Global for when removing comments from curLine
bool inComment = false;
//Global ints for keeping track of line count, and if user provided a file or choses command line input
int lineNum = 0, userInput;
//String storing token names for print tokens function
string tokenNames[41] = {" ", "Identifier", "Number", "Assignment Operator", "Greater Than", "Less Than", "Equal To", "Colon", "Colon Equal", "Plus", "Minus", "Multiplication", "Divison", "Mod", "Dot", "Left Parentheses", "Right Parentheses", "Comma", "L. Curly Bracket", "R. Curly Bracket", "SemiColon", "L. Square Bracket", "R. Square Bracket", "Keyword", "Start", "Stop", "Loop", "While", "For", "Label", "Exit", "Listen", "Talk", "Program", "If", "Then", "Assign", "Declare", "Jump", "Else", "EOF" };
enum states {s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s20, s21, s22, s23};
string keyword[] = {"start", "stop", "loop", "while", "for", "label", "exit", "listen", "talk", "program", "if", "then", "assign", "declare", "jump", "else"};
ifstream input;

//Main driver function to read through input and FSA table
vector<Token> FSADriver(string file)
{
    //50 is a final state, -1 is an error state
    int FSATable[23][25] = {
            //                        WS     a-z     A-Z    0-9      $      =       >      <      :     +      -      *      /      %      .      (      )      ,      {      }      ;      [     ]     EOF   Unknown
            /* Whitespace      */ {   s1,    s2,      -1,    s3,     s2,    s4,    s5,    s6,    s8,   s10,   s11,   s12,   s13,   s14,   s15,   s16,   s17,   s18,   s19,   s20,   s21,   s22,   s23,    50, -1},
            /* Identifier      */ {   50,    s2,      s2,    s2,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Number          */ {   50,    50,      50,    s3,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Assignment      */ {   50,    50,      50,    50,     50,    s7,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Greater Than    */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Less Than       */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Equal           */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Colon           */ {   50,    50,      50,    50,     50,    s9,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Colon Equal     */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Plus            */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Minus           */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Multi           */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Divison         */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Mod             */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Dot             */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Left Par        */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Right Par       */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* Comma           */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* L C Bracket     */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* R C Bracket     */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* SemiColon       */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* L S Bracket     */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
            /* R S Bracket     */ {   50,    50,      50,    50,     50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50,    50, -1},
    };

    string filteredString = "";
    vector<Token> tokenContainer;
    Token curToken;
    strContainer curLine;
    int curState, lookahead, curCol, curLineLen;

    //Check if user provided inputfile or not
    if (file == "")
    {
        userInput = 1;
    }
    else
    {
        userInput = 2;
    }

    //If userInput is 2, they provided a file that needs to be opened
    if (userInput == 2)
    {
	    openFile(file);
    }

    //Read lines until end of file	
	while (curLine.eof == false)
	{
        //Read lines from input and store them in the strContainer to keep track of lineText, lineNum, and the EOF bool
        curLine = readLine(input);
        curLineLen = curLine.lineText.length();
        //If we havent reach eof yet
        if (curLine.eof == false)
        {
            //Initialize current state and the string to hol
            curState = s1;
            filteredString = "";
            //Go through the string, look into FSATable, and turn the string into tokens
            for (int x = 0; x <= curLineLen - 1; x++)
            {
                if (curLine.lineText[x] == '\r' || curLine.lineText[x] == '\n')
                {
                    break;
                }
                curCol = findColumnNum(curLine.lineText[x]);
                lookahead = FSATable[curState][curCol];
                //If the lookahead shows an error state, end the program
                if (lookahead == -1)
                {
                    errorMessage();
                }
                //If the lookahead shows a final state OR the identifier is longer than 8 chars
                else if (lookahead == 50 || filteredString.length() == 8 && tokenID(curState) == IDENT_tk)
                {
                    //We need to check if the filteredString is a keyword or not
                    if (checkForKeyword(filteredString))
                    {
                        //Generate keyword token and add it to the vector
                        curToken = identifyKeyword(filteredString);
                        tokenContainer.push_back(curToken);
                    }
                    else
                    {
                        //Generate token for whatever else was read in filteredString
                        curToken = createToken(filteredString, tokenID(curState), lineNum);
                        tokenContainer.push_back(curToken);
                    }
                    //Reinitialize state and filter for reading new tokens
                    curState = s1;
                    filteredString = "";
                    lookahead = FSATable[curState][curCol];
                    //If lookahead is error, end program
                    if (lookahead == -1)
                    {
                        errorMessage();
                    }
                    //If current char read isn't whitespace, add it to filtered string
                    if (curCol != WS)
                    {
                        filteredString = filteredString + curLine.lineText[x];
                    }
                    //If lookahead isn't a final state, update current state
                    if (lookahead != 50)
                    {
                        curState = lookahead;
                    }
                }
                //Char isn't whitespace, add it to filter
                else
                {
                    if (curCol != WS)
                    {
                        filteredString = filteredString + curLine.lineText[x];
                        curState = lookahead;
                    }
                }
            }
            //If filteredString isn't empty, tokenize it
            if (filteredString != "")
            {
                //Check for keyword
                if (checkForKeyword(filteredString))
                {
                    curToken = identifyKeyword(filteredString);
                    tokenContainer.push_back(curToken);
                }
                //Tokenize string
                else
                {
                    curToken = createToken(filteredString, tokenID(curState), lineNum);
                    tokenContainer.push_back(curToken);
                }
            }
        }
	}
    //Add EOF Token to vector as the file is done being read without any errors
    curToken = createToken(tokenNames[40], tokenID(40), lineNum);
    tokenContainer.push_back(curToken);

    input.close();
    cout << "Scanner is complete!" << endl;
    return tokenContainer;
}

//Create new token object
Token createToken(string tokenText, tokenID tokenType, int lNum)
{
    Token newToken;
    newToken.TokenInstance = tokenText;
    newToken.tokenClass = tokenType;
    newToken.lineNum = lNum;
    return newToken;
}

//Output tokens stored in vector, basically testScanner
void printTokens(vector<Token> tokenList)
{
    cout << "Printing Token Information: " << endl;
    for (int i = 0; i < tokenList.size(); i++)
    {
        cout << "Token Number: " << i << " {";
        cout << "Token ID: " << tokenNames[tokenList.at(i).tokenClass] << ", ";
        cout << "Token Instance: " << tokenList.at(i).TokenInstance << ", ";
        cout << "Token Line Number: " << tokenList.at(i).lineNum << "}" << endl;
    }
}

//Read line from file or userinput
strContainer readLine(ifstream &input)
{
    strContainer Line;
    while(true)
    {
        //If userInput is 2, a file was provided, so we can getline from the file
        if(userInput == 2)
        {
            if (getline(input, Line.lineText))
            {
                //Update strContainer lineNum, and text
                Line.lineNum = lineNum;
                lineNum++;
                //Remove comments from the line
                Line.lineText = filterComment(Line.lineText);
                //Check if line became blank after input filter
                if (whitespaceLine(Line.lineText) == false)
                {
                    break;
                }
            }
            //Not possible to get more lines, end of file
            else
            {
                Line.eof = true;
                break;
            }
        }
        //If user input is 1, no input was provided
        else if(userInput == 1)
        {
            //getline and cin allow for multiline user input
            if (getline(cin, Line.lineText))
            {
                Line.lineNum = lineNum;
                lineNum++;
                Line.lineText = filterComment(Line.lineText);
                //Check if line became blank after input filter
                if (whitespaceLine(Line.lineText) == false)
                {
                    break;
                }
            }
            else
            {
                Line.eof = true;
                break;
            }
        }
    }

    return Line;
}

//Check if string is blank
bool whitespaceLine(string filteredLine)
{
    for (int x = 0; x < filteredLine.length(); x++)
    {
        if (filteredLine[x] != ' ' && filteredLine[x] != '\n')
        {
            return false;
        }
    }

    return true;
}

//Open file function
void openFile(string inputFile) 
{
    input.open(inputFile, ios::in);

    if (input.is_open() == false)
    {
        input.close();
    }
}

//Output error message and exit
void errorMessage()
{
    cout << "Error reading file! Ending program" << endl;
    exit(0);
}

//Remove comments
std::string filterComment(string curString)
{
    string noCommentString;

    //Remove comments
    for(int i = 0; i < curString.length(); i++)
    {
        //If in comment, check for end
        if(inComment == true && curString[i] == '&' && curString[i+1] == '&')
        {
            inComment = false;
            //Since this is the end of the comment, we increment i one extra time to get past the second &
            i++;
        }
        else if (inComment)
        {
            continue;
        }
        else if (curString[i] == '&' && curString[i+1] == '&')
        {
            inComment = true;
        }
        else
        {
            noCommentString = noCommentString + curString[i];
        }
    }

    return noCommentString;
}

//See if identifier is a keyword
bool checkForKeyword(string tokenText)
{
    for (int x = 0; x < 16; x++)
    {
        if (tokenText == keyword[x])
        {
            return true;
        }
    }
    
    return false;
}

Token identifyKeyword(string keywordText)
{
    Token keywordToken;
    if (keywordText == "start")
    {
        keywordToken = createToken(keywordText, START_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "stop")
    {
        keywordToken = createToken(keywordText, STOP_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "loop")
    {
        keywordToken = createToken(keywordText, LOOP_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "while")
    {
        keywordToken = createToken(keywordText, WHILE_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "for")
    {
        keywordToken = createToken(keywordText, FOR_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "label")
    {
        keywordToken = createToken(keywordText, LABEL_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "exit")
    {
        keywordToken = createToken(keywordText, EXIT_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "listen")
    {
        keywordToken = createToken(keywordText, LISTEN_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "talk")
    {
        keywordToken = createToken(keywordText, TALK_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "program")
    {
        keywordToken = createToken(keywordText, PROGRAM_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "if")
    {
        keywordToken = createToken(keywordText, IF_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "then")
    {
        keywordToken = createToken(keywordText, THEN_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "assign")
    {
        keywordToken = createToken(keywordText, ASSIGN_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "declare")
    {
        keywordToken = createToken(keywordText, DECLARE_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "jump")
    {
        keywordToken = createToken(keywordText, JUMP_tk, lineNum);
        return keywordToken;
    }
    else if (keywordText == "else")
    {
        keywordToken = createToken(keywordText, ELSE_tk, lineNum);
        return keywordToken;
    }
    else
    {
        keywordToken = createToken(keywordText, KEYW_tk, lineNum);
        return keywordToken;
    }
}

//Return proper column enum for char read from filteredString for FSAtable
int findColumnNum(char c)
{
    if (c == ' ')
    {
        return WS;
    }
    else if (c == 'a')
    {
        return LOW_Char;
    }
    else if (c == 'b')
    {
        return LOW_Char;
    }
    else if (c == 'c')
    {
        return LOW_Char;
    }
    else if (c == 'd')
    {
        return LOW_Char;
    }
    else if (c == 'e')
    {
        return LOW_Char;
    }
    else if (c == 'f')
    {
        return LOW_Char;
    }
    else if (c == 'g')
    {
        return LOW_Char;
    }
    else if (c == 'h')
    {
        return LOW_Char;
    }
    else if (c == 'i')
    {
        return LOW_Char;
    }
    else if (c == 'j')
    {
        return LOW_Char;
    }
    else if (c == 'k')
    {
        return LOW_Char;
    }
    else if (c == 'l')
    {
        return LOW_Char;
    }
    else if (c == 'm')
    {
        return LOW_Char;
    }
    else if (c == 'n')
    {
        return LOW_Char;
    }
    else if (c == 'o')
    {
        return LOW_Char;
    }
    else if (c == 'p')
    {
        return LOW_Char;
    }
    else if (c == 'q')
    {
        return LOW_Char;
    }
    else if (c == 'r')
    {
        return LOW_Char;
    }
    else if (c == 's')
    {
        return LOW_Char;
    }
    else if (c == 't')
    {
        return LOW_Char;
    }
    else if (c == 'u')
    {
        return LOW_Char;
    }
    else if (c == 'v')
    {
        return LOW_Char;
    }
    else if (c == 'w')
    {
        return LOW_Char;
    }
    else if (c == 'x')
    {
        return LOW_Char;
    }
    else if (c == 'y')
    {
        return LOW_Char;
    }
    else if (c == 'z')
    {
        return LOW_Char;
    }
    else if (c == 'A')
    {
        return Up_Char;
    }
    else if (c == 'B')
    {
        return Up_Char;
    }
    else if (c == 'C')
    {
        return Up_Char;
    }
    else if (c == 'D')
    {
        return Up_Char;
    }
    else if (c == 'E')
    {
        return Up_Char;
    }
    else if (c == 'F')
    {
        return Up_Char;
    }
    else if (c == 'G')
    {
        return Up_Char;
    }
    else if (c == 'H')
    {
        return Up_Char;
    }
    else if (c == 'I')
    {
        return Up_Char;
    }
    else if (c == 'J')
    {
        return Up_Char;
    }
    else if (c == 'K')
    {
        return Up_Char;
    }
    else if (c == 'L')
    {
        return Up_Char;
    }
    else if (c == 'M')
    {
        return Up_Char;
    }
    else if (c == 'N')
    {
        return Up_Char;
    }
    else if (c == 'O')
    {
        return Up_Char;
    }
    else if (c == 'P')
    {
        return Up_Char;
    }
    else if (c == 'Q')
    {
        return Up_Char;
    }
    else if (c == 'R')
    {
        return Up_Char;
    }
    else if (c == 'S')
    {
        return Up_Char;
    }
    else if (c == 'T')
    {
        return Up_Char;
    }
    else if (c == 'U')
    {
        return Up_Char;
    }
    else if (c == 'V')
    {
        return Up_Char;
    }
    else if (c == 'W')
    {
        return Up_Char;
    }
    else if (c == 'X')
    {
        return Up_Char;
    }
    else if (c == 'Y')
    {
        return Up_Char;
    }
    else if (c == 'Z')
    {
        return Up_Char;
    }
    else if (c == '0')
    {
        return Num;
    }
    else if (c == '1')
    {
        return Num;
    }
    else if (c == '2')
    {
        return Num;
    }
    else if (c == '3')
    {
        return Num;
    }
    else if (c == '4')
    {
        return Num;
    }
    else if (c == '5')
    {
        return Num;
    }
    else if (c == '6')
    {
        return Num;
    }
    else if (c == '7')
    {
        return Num;
    }
    else if (c == '8')
    {
        return Num;
    }
    else if (c == '9')
    {
        return Num;
    }
    else if (c == '$')
    {
        return DollarSign;
    }
    else if (c == '=')
    {
        return Assign;
    }
    else if (c == '>')
    {
        return GThen;
    }
    else if (c == '<')
    {
        return LThen;
    }
    else if (c == ':')
    {
        return Colon;
    }
    else if (c == '+')
    {
        return Plus;
    }
    else if (c == '-')
    {
        return Minus;
    }
    else if (c == '*')
    {
        return Multi;
    }
    else if (c == '/')
    {
        return Div;
    }
    else if (c == '%')
    {
        return Mod;
    }
    else if (c == '.')
    {
        return Period;
    }
    else if (c == '(')
    {
        return LParenth;
    }
    else if (c == ')')
    {
        return RParenth;
    }
    else if (c == ',')
    {
        return Comma;
    }
    else if (c == '{')
    {
        return LCBracket;
    }
    else if (c == '}')
    {
        return RCBracket;
    }
    else if (c == ';')
    {
        return SemiColon;
    }
    else if (c == '[')
    {
        return LSBracket;
    }
    else if (c == ']')
    {
        return RSBracket;
    }
    else
    {
        return Unknown;
    }
}