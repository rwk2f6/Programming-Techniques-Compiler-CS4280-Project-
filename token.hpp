#ifndef TOKEN_HPP
#define TOKEN_HPP
#include <string>
#include <vector>

using namespace std;

enum tokenID {WhiteSpace, IDENT_tk, NUM_tk, ASSOP_tk, GTHEN_tk, LTHEN_tk, EQUAL_tk, COLON_tk, COLONEQ_tk, PLUS_tk, MINUS_tk, MULT_tk, DIV_tk, MOD_tk, PERIOD_tk, LPARENTH_tk, RPARENTH_tk, COMMA_tk, LCBRACKET_tk, RCBRACKET_tk, SEMIC_tk, LSBRACKET_tk, RSBRACKET_tk, KEYW_tk, START_tk, STOP_tk, LOOP_tk, WHILE_tk, FOR_tk, LABEL_tk, EXIT_tk, LISTEN_tk, TALK_tk, PROGRAM_tk, IF_tk, THEN_tk, ASSIGN_tk, DECLARE_tk, JUMP_tk, ELSE_tk, EOF_tk };
enum columns {WS, LOW_Char, Up_Char, Num, DollarSign, Assign, GThen, LThen, Colon, Plus, Minus, Multi, Div, Mod, Period, LParenth, RParenth, Comma, LCBracket, RCBracket, SemiColon, LSBracket, RSBracket, eof, Unknown};

struct Token
{
	tokenID tokenClass;
    string TokenInstance;
    int lineNum;
	//Default constructor
	Token()
	{
		tokenClass = WhiteSpace;
		TokenInstance = "";
		lineNum = 0;
	}

	//Constructor
	Token (tokenID tokenType, string instance, int line)
	{
		tokenClass = tokenType;
        TokenInstance = instance;
        lineNum = line;
	}
};

Token createToken(std::string, tokenID, int);

#endif