#include "../inc/line_analyzer.h"
int line_analyzer::hextoint(std::string s)
{
	int x;
	std::stringstream ss;
	ss << std::hex << s;
	ss >> x;
	return static_cast<int>(x);
}
std::vector<std::string> line_analyzer::split(std::string str, char delimeter)
{
	std::vector<std::string> internal;
	std::stringstream ss(str); // Turn the string into a stream. 
	std::string tok;
	while (std::getline(ss, tok, delimeter))
		internal.push_back(removeSpaces(tok));
	return internal;
}

std::string line_analyzer::removeSpaces(std::string str)
{
	str.erase(remove(str.begin(), str.end(), ' '), str.end());
	return str;
}
int line_analyzer::calculate_location_counter_for_instruction(std::string line)
{
	if (isInstructionNoOperand(line)) return 1;
	std::string instruction_name = line_analyzer::getInstructionName(line); // pazi mozda ne uhvati ime instrukcije lepo
	if (isInstructionOneOperand(line)) {
		if (instruction_name == "int" || instruction_name == "not") 
			return 2;
		else if (instruction_name == "push" || instruction_name == "pop") 
			return 3;
	}
	if (isInstructionTwoOperands(line))
		return 2;
	
	if (isLdstrRegDir(line) || isLdstrRegInd(line) || 
		isJumpRegDir(line) || isJumpRegInd(line)) 
			return 3;

	if (isLdstrMemdirAbsolute(line) || isLdstrPC(line) || isLdstrDirectLiteral(line) || isLdstrDirectSymbol(line) || isLdstrDisplLiteral(line) || isLdstrDisplSymbol(line) ||
		isJumpAbsolute(line) || isJumpPC(line) || isJumpMemdirLiteral(line) || isJumpMemdirSymbol(line) || isJumpDisplLiteral(line) || isJumpDisplSymbol(line)) 
		return 5;
	return 0;
}
std::string line_analyzer::getInstructionName(std::string line)
{
	std::regex regex_instruction_name(regex_rules::regex_instruction_name);
	std::smatch match;
	std::regex_match(line, match, regex_instruction_name);
	std::string name = match.str(1);
	return name;
}
std::string line_analyzer::removeComments(std::string line)
{
	int endIndex = line.find("#");
	if (endIndex != -1) return line.substr(0, endIndex);

	return line;
}
int line_analyzer::getInstructionCode(std::string line)
{
	return getInstructionCodeByInstructionName(getInstructionName(line));
}
/*
	one operand instruction
	two operand instruction 

	jump regdir
	jump reging

	static std::string regex_instruction_ldstr_direct_literal;
	static std::string regex_instruction_ldstr_direct_symbol;
	static std::string regex_instruction_ldstr_regdir;
	static std::string regex_instruction_ldstr_regind;
	static std::string regex_instruction_ldstr_absolute;
	static std::string regex_instruction_ldstr_pc;
*/
int line_analyzer::getRegisterCodeFirstArgument(std::string line)
{
	if (isInstructionOneOperand(line) || isInstructionTwoOperands(line) || isJumpRegDir(line) 
		|| isJumpRegInd(line) || isLdstr(line)) {
		// make a general regex for these to catch the code for first register
		std::regex regex_catch_register(regex_rules::regex_catch_first_register);
		std::smatch match;
		if (std::regex_match(line, match, regex_catch_register))
			return (match.str(2) == "psw") ? 8 : (match.str(2).at(1) - '0');
	}
	return 0;
}

int line_analyzer::getRegisterCodeSecondArgument(std::string line)
{
	if (isInstructionTwoOperands(line) || isLdstrRegDir(line) || isLdstrRegInd(line)) {
		std::regex regex_catch_register(regex_rules::regex_catch_second_register);
		std::smatch match;
		if (std::regex_match(line, match, regex_catch_register))
			return (match.str(3) == "psw") ? 8 : (match.str(3).at(1) - '0');
	}
	return 0;
}

int line_analyzer::getRegisterCodeThirdArgument(std::string line)
{
	if (isInstructionTwoOperands(line) || isLdstrRegDir(line) || isLdstrRegInd(line)) {
		std::regex regex_catch_register(regex_rules::regex_catch_second_register);
		std::smatch match;
		if (std::regex_match(line, match, regex_catch_register))
			return (match.str(4) == "psw") ? 8 : (match.str(3).at(1) - '0');
	}
	return 0;
}



int line_analyzer::getInstructionCodeByInstructionName(std::string code)
{
		if (code == "iret")return 0x20;
		if (code == "ret")return 0x40;
		if (code == "int")return 0x10;
		if (code == "call")return 0x30;
		if (code == "jmp")return 0x50;
		if (code == "jeq")return 0x51;
		if (code == "jne")return 0x52;
		if (code == "jgt")return 0x53;
		if (code.compare("ldr")==0)return 0xA0;
		if (code == "shr")return 0x91;
		if (code == "str")return 0x80;
		if (code == "push")return 0xB0;
		if (code == "pop")return 0xA0;
		if (code == "xchg")return 0x60;
		if (code == "add")return 0x70;
		if (code == "sub")return 0x71;
		if (code == "mul")return 0x72;
		if (code == "div")return 0x73;
		if (code == "cmp")return 0x74;
		if (code == "not")return 0x80;
		if (code == "and")return 0x81;
		if (code == "or")return 0x82;
		if (code == "xor")return 0x83;
		if (code == "test")return 0x84;
		if (code == "shl")return 0x90;
		return 0x00;
}
int line_analyzer::getLiteralFromSkip(std::string line)
{
	std::regex regex_skip(regex_rules::regex_skip);
	std::smatch skip;
	std::string s;
	std::regex_match(line, skip, regex_skip);
	int value_for_skipping = std::stoi(skip.str(1));
	return value_for_skipping;
}
std::string line_analyzer::getLabelName(std::string line)
{
	std::regex regex_label(regex_rules::regex_label);
	std::smatch label;
	std::regex_match(line, label, regex_label);
	std::string labelName = label.str(1);
	return labelName;
}
std::string line_analyzer::getSectionName(std::string line)
{
	std::regex regex_section(regex_rules::regex_section);
	std::smatch section;
	std::regex_match(line, section, regex_section);
	std::string sectionName = section.str(1);
	return sectionName;
}
std::string line_analyzer::getGlobalName(std::string line)
{
	std::regex regex_global(regex_rules::regex_global);
	std::smatch section;
	std::regex_match(line, section, regex_global);
	std::string globalName = section.str(1);
	return globalName;
}
std::string line_analyzer::getExternName(std::string line)
{
	std::regex regex_extern(regex_rules::regex_extern);
	std::smatch section;
	std::regex_match(line, section, regex_extern);
	std::string externName = section.str(0).substr(8, section.length() - 1);
	return externName;
}
int line_analyzer::getWordLiteral(std::string line)
{
	if (isWordInteger(line)) {
		std::regex regex_wordint(regex_rules::regex_word_integer);
		std::smatch match;
		std::regex_match(line, match, regex_wordint);
		return stoi(match.str(1));
	}
	else if (isWordHex(line)) {
		std::regex regex_wordhex(regex_rules::regex_word_hex);
		std::smatch match;
		std::regex_match(line, match, regex_wordhex);
		return stoi(match.str(1));
	}
	return -1;
}
std::string line_analyzer::getWordSymbolName(std::string line)
{
	std::regex regex_word_symbol(regex_rules::regex_word_symbol);
	std::smatch match;
	std::regex_match(line, match, regex_word_symbol);
	return match.str(1);
}
int line_analyzer::getEquValue(std::string line)
{
	std::regex equregex(regex_rules::regex_equ);
	std::smatch match;
	std::regex_match(line, match, equregex);
	std::string val = match.str(2);
	int value = stoi(val, nullptr, 0);
	return value;
}
std::string line_analyzer::getEquName(std::string line)
{
	std::regex equregex(regex_rules::regex_equ);
	std::smatch match;
	std::regex_match(line, match, equregex);
	std::string symbolName = match.str(1);
	return symbolName;
}
bool line_analyzer::isEmpty(std::string line)
{
	std::regex regex_empty(regex_rules::regex_empty);
	if (std::regex_match(line.begin(), line.end(), regex_empty)) return true;
	return false;
}

bool line_analyzer::isLabel(std::string line)
{
	std::regex regex_label(regex_rules::regex_label);
	return std::regex_match(line.begin(), line.end(), regex_label);
}

bool line_analyzer::isEqu(std::string line)
{
	std::regex regex_equ(regex_rules::regex_equ);
	return std::regex_match(line.begin(), line.end(), regex_equ);
}

bool line_analyzer::isSection(std::string line)
{
	std::regex regex_section(regex_rules::regex_section);
	return std::regex_match(line.begin(), line.end(), regex_section);
}

bool line_analyzer::isGlobal(std::string line)
{
	std::regex regex_global(regex_rules::regex_global);
	return std::regex_match(line.begin(), line.end(), regex_global);
}

bool line_analyzer::isExtern(std::string line)
{
	std::regex regex_extern(regex_rules::regex_extern);
	return std::regex_match(line.begin(), line.end(), regex_extern);
}

bool line_analyzer::isWord(std::string line)
{
	std::regex regex_word(regex_rules::regex_word);
	return regex_match(line.begin(), line.end(), regex_word);
}

bool line_analyzer::isWordInteger(std::string line)
{
	std::regex regex_integer(regex_rules::regex_word_integer);
	return regex_match(line.begin(), line.end(), regex_integer);
}

bool line_analyzer::isWordHex(std::string line)
{
	std::regex regex_hex(regex_rules::regex_word_hex);
	return regex_match(line.begin(), line.end(), regex_hex);
}

bool line_analyzer::isWordSymbol(std::string line)
{
	std::regex regex_symbol(regex_rules::regex_word_symbol);
	return regex_match(line.begin(), line.end(), regex_symbol);
}

bool line_analyzer::isSkip(std::string line)
{
	std::regex regex_skip(regex_rules::regex_skip);
	return regex_match(line.begin(), line.end(), regex_skip);
}

bool line_analyzer::isEnd(std::string line)
{
	std::regex regex_end(regex_rules::regex_end);
	return regex_match(line.begin(), line.end(), regex_end);
}

//instructions

bool line_analyzer::isInstruction(std::string line)
{
	return isInstructionNoOperand(line) ||
		isInstructionOneOperand(line) ||
		isInstructionTwoOperands(line) ||
		isJump(line) ||
		isLdstr(line);
}

bool line_analyzer::isInstructionNoOperand(std::string line)
{
	std::regex regex_instruction_no_operand(regex_rules::regex_instruction_no_operand);
	return regex_match(line.begin(), line.end(), regex_instruction_no_operand);
}

bool line_analyzer::isInstructionOneOperand(std::string line)
{
	std::regex regex_instruction_one_operand(regex_rules::regex_instruction_one_operand);
	return regex_match(line.begin(), line.end(), regex_instruction_one_operand);
}

bool line_analyzer::isInstructionTwoOperands(std::string line)
{
	std::regex regex_instruction_two_operands(regex_rules::regex_instruction_two_operands);
	return regex_match(line.begin(), line.end(), regex_instruction_two_operands);
}


//Jump instruction

bool line_analyzer::isJump(std::string line) {
	return isJumpDirect(line) ||
		isJumpRegDir(line) ||
		isJumpRegInd(line) ||
		isJumpPC(line) ||
		isJumpAbsolute(line) ||
		isJumpMemdirSymbol(line) ||
		isJumpMemdirLiteral(line) ||
		isJumpDisplLiteral(line) ||
		isJumpDisplSymbol(line);
}

bool line_analyzer::isJumpDirect(std::string line)
{
	std::regex regex_instruction_jmp_direct(regex_rules::regex_instruction_jmp_direct);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_direct);
}

bool line_analyzer::isJumpRegDir(std::string line)
{
	std::regex regex_instruction_jmp_regdir(regex_rules::regex_instruction_jmp_regdir);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_regdir);
}

bool line_analyzer::isJumpRegInd(std::string line)
{
	std::regex regex_instruction_jmp_regind(regex_rules::regex_instruction_jmp_regind);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_regind);
}

bool line_analyzer::isJumpPC(std::string line)
{
	std::regex regex_instruction_jmp_pc(regex_rules::regex_instruction_jmp_pc);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_pc);
}

bool line_analyzer::isJumpAbsolute(std::string line)
{
	std::regex regex_instruction_jmp_absolute(regex_rules::regex_instruction_jmp_absolute);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_absolute);
}

bool line_analyzer::isJumpMemdirSymbol(std::string line)
{
	std::regex regex_instruction_jmp_memdir_symbol(regex_rules::regex_instruction_jmp_memdir_symbol);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_memdir_symbol);
}

bool line_analyzer::isJumpMemdirLiteral(std::string line)
{
	std::regex regex_instruction_jmp_memdir_literal(regex_rules::regex_instruction_jmp_memdir_literal);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_memdir_literal);
}

bool line_analyzer::isJumpDisplLiteral(std::string line)
{
	std::regex regex_instruction_jmp_displ_literal(regex_rules::regex_instruction_jmp_displ_literal);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_displ_literal);
}

bool line_analyzer::isJumpDisplSymbol(std::string line)
{
	std::regex regex_instruction_jmp_displ_symbol(regex_rules::regex_instruction_jmp_displ_symbol);
	return regex_match(line.begin(), line.end(), regex_instruction_jmp_displ_symbol);
}

// check if LD/STR


bool line_analyzer::isLdstr(std::string line)
{
	return isLdstrDirectLiteral(line) ||
		isLdstrDirectSymbol(line) ||
		isLdstrRegDir(line) ||
		isLdstrRegInd(line) ||
		isLdstrMemdirAbsolute(line) ||
		isLdstrPC(line) ||
		isJumpDisplLiteral(line) ||
		isJumpDisplSymbol(line);
}

bool line_analyzer::isLdstrDirectLiteral(std::string line)
{
	std::regex regex_instruction_ldstr_direct_literal(regex_rules::regex_instruction_ldstr_direct_literal);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_direct_literal);
}

bool line_analyzer::isLdstrDirectSymbol(std::string line)
{
	std::regex regex_instruction_ldstr_direct_symbol(regex_rules::regex_instruction_ldstr_direct_symbol);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_direct_symbol);
}

bool line_analyzer::isLdstrRegDir(std::string line)
{
	std::regex regex_instruction_ldstr_regdir(regex_rules::regex_instruction_ldstr_regdir);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_regdir);
}

bool line_analyzer::isLdstrRegInd(std::string line)
{
	std::regex regex_instruction_ldstr_regind(regex_rules::regex_instruction_ldstr_regind);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_regind);
}

bool line_analyzer::isLdstrMemdirAbsolute(std::string line)
{
	std::regex regex_instruction_ldstr_memdir_absolute(regex_rules::regex_instruction_ldstr_memdir_absolute);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_memdir_absolute);
}

bool line_analyzer::isLdstrMemdirLiteral(std::string line)
{
	std::regex regex_instruction_ldstr_memdir_literal(regex_rules::regex_instruction_ldstr_memdir_literal);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_memdir_literal);
}

bool line_analyzer::isLdstrPC(std::string line)
{
	std::regex regex_instruction_ldstr_pc(regex_rules::regex_instruction_ldstr_pc);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_pc);
}

bool line_analyzer::isLdstrDisplLiteral(std::string line)
{
	std::regex regex_instruction_ldstr_displ_literal(regex_rules::regex_instruction_jmp_displ_literal);
	return regex_match(line.begin(), line.end(), regex_instruction_ldstr_displ_literal);
}

bool line_analyzer::isLdstrDisplSymbol(std::string line)
{
	std::regex regex_instruction_displ_symbol(regex_rules::regex_instruction_ldstr_displ_symbol);
	return regex_match(line.begin(), line.end(), regex_instruction_displ_symbol);
}




