#ifndef _line_analyzer_h_
#define _line_analyzer_h_
#include <iostream>
#include <sstream>
#include "regex_rules.h"
class line_analyzer
{
private:
	static int getInstructionCodeByInstructionName(std::string code);
public:

	// pomocne funkcije
	static int hextoint(std::string s);
	static std::vector<std::string> split(std::string line, char delimeter);
	static std::string removeSpaces(std::string str);

	static int calculate_location_counter_for_instruction(std::string line);
	static std::string getInstructionName(std::string line);
	static std::string removeComments(std::string line);
	static int getInstructionCode(std::string line); // get instruction code by whole line
	static int getRegisterCodeFirstArgument(std::string line); // Figures out what register was used as a first argument, and returns its byte code. Works for any type of instruction.
	static int getRegisterCodeSecondArgument(std::string line);

	//get-eri
	static int getLiteralFromSkip(std::string line);
	static std::string getLabelName(std::string line);
	static std::string getSectionName(std::string line);
	static std::string getGlobalName(std::string line);
	static std::string getExternName(std::string line);
	static int getWordLiteral(std::string line);
	static std::string getWordSymbolName(std::string line);
	static int getEquValue(std::string line);
	static std::string getEquName(std::string line);

	// check if's
	static bool isEmpty(std::string line);
	static bool isLabel(std::string line);
	static bool isEqu(std::string line);
	static bool isSection(std::string line);
	static bool isGlobal(std::string line);
	static bool isExtern(std::string line);
	static bool isWord(std::string line);
	static bool isWordInteger(std::string line);
	static bool isWordHex(std::string line);
	static bool isWordSymbol(std::string line);
	static bool isSkip(std::string line);
	static bool isEnd(std::string line);

	//check if instruction
	static bool isInstruction(std::string line);

	static bool isInstructionNoOperand(std::string line);
	static bool isInstructionOneOperand(std::string line);
	static bool isInstructionTwoOperands(std::string line);

	// check if jump
	static bool isJump(std::string line); // is it jump instruction
	static bool isJumpDirect(std::string line);
	static bool isJumpRegDir(std::string line);
	static bool isJumpRegInd(std::string line);
	static bool isJumpPC(std::string line);
	static bool isJumpAbsolute(std::string line);
	static bool isJumpMemdirSymbol(std::string line);
	static bool isJumpMemdirLiteral(std::string line);

	//check if ld/str
	static bool isLdstr(std::string line);
	static bool isLdstrDirectLiteral(std::string line);
	static bool isLdstrDirectSymbol(std::string line);
	static bool isLdstrRegDir(std::string line);
	static bool isLdstrRegInd(std::string line);
	static bool isLdstrMemdirAbsolute(std::string line);
	static bool isLdstrMemdirLiteral(std::string line);
	static bool isLdstrPC(std::string line);

	// calculate codes


};
#endif

