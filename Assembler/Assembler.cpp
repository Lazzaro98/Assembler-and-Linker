#include "Assembler.h"

void Assembler::processLabel1(std::string line)
{
	if (!line_analyzer::isLabel(line)) return;
	std::string labelName = line_analyzer::getLabelName(line);

	symbol& s = ST->find(labelName);
	if (s.name == "NOT_FOUND")
		ST->add(labelName, current_section, location_counter, 0, false, true);
	else {
		if (s.is_defined)
			throw "Symbol " + s.name + " already defined.";
		else {
			s.is_defined = true;
			s.value = location_counter;
			s.section = current_section;
		}
	}
}

void Assembler::processSection1(std::string line)
{
	if (!line_analyzer::isSection(line)) return;
	std::string sectionName = line_analyzer::getSectionName(line);

	if (this->current_section != "") //zavrsi prethodnu sekciju
		ST->find(this->current_section).size = this->location_counter;

	symbol& s = ST->find(sectionName);
	if (s.name != "NOT_FOUND")
		throw "Symbol " + s.name + " already defined.";
	else {
		if (sectionName != "") {
			ST->add(sectionName, sectionName, 0, 0, false, false); // TODO: is_defined = true ?
			SL->add(sectionName, ST->find(sectionName).id);
		}

		this->current_section = sectionName;
		this->location_counter = 0;
	}
}

void Assembler::processGlobal1(std::string line)
{
	if (!line_analyzer::isGlobal(line))return;
	std::string globalName1 = line_analyzer::getGlobalName(line);

	std::vector<std::string> list = line_analyzer::split(globalName1, ',');

	for (auto& globalName : list) {
		symbol& s = ST->find(globalName);
		if (s.name == "NOT_FOUND")
			ST->add(globalName, "", 0, 0, true, false);
		else
			s.is_global = true;
	}
}

void Assembler::processExtern1(std::string line)
{
	if (!line_analyzer::isExtern(line))return;
	std::string externName1 = line_analyzer::getExternName(line);

	std::vector<std::string> list = line_analyzer::split(externName1, ',');

	for (auto& externName : list) {
		symbol& s = ST->find(externName);
		if (s.name == "NOT_FOUND")
			ST->add(externName, "UNDEFINED", 0, 0, true, false);
		else {
			throw "Symbol " + s.name + " already defined.";
		}
	}
}

void Assembler::processSkip1(std::string line)
{
	if (!line_analyzer::isSkip(line))return;
	int value_for_skipping = line_analyzer::getLiteralFromSkip(line);

	this->location_counter += value_for_skipping;
}

void Assembler::processWord1(std::string line)
{
	if (!line_analyzer::isWord(line))return;
	int number_of_commas = std::count(line.begin(), line.end(), ','); //if it's a list of literals

	this->location_counter += (2 * (number_of_commas + 1));
}

void Assembler::processEqu1(std::string line)
{
	if (!line_analyzer::isEqu(line))return;
	int value = line_analyzer::getEquValue(line);
	std::string symbolName = line_analyzer::getEquName(line);
	symbol& s = ST->find(symbolName);

	if (s.name == "NOT_FOUND")
		ST->add(symbolName, "ABSOLUTE", value, 0, true, true); // TODO: is_defined = true?
	else
		throw "Symbol " + s.name + " already defined.";
}

void Assembler::processInstruction1(std::string line)
{
	if (!line_analyzer::isInstruction(line)) return;
	this->location_counter += line_analyzer::calculate_location_counter_for_instruction(line);
}

void Assembler::processLine1(std::string line)
{
	line = line_analyzer::removeComments(line);
	if (line_analyzer::isEmpty(line)) return;
	processLabel1(line);
	processEqu1(line);
	processSection1(line);
	processGlobal1(line);
	processExtern1(line);
	processWord1(line);
	processSkip1(line);
	processEnd1(line);
	processInstruction1(line);
}

void Assembler::processLabel2(std::string label)
{
	// we do nothing when we pass label during 2nd pass :)
}

void Assembler::processSection2(std::string line)
{
	if (!line_analyzer::isSection(line))return;
	this->location_counter = 0;
	this->current_section = line_analyzer::getSectionName(line);
}

void Assembler::processGlobal2(std::string line)
{
	std::string globalName1 = line_analyzer::getGlobalName(line);

	std::vector<std::string> list = line_analyzer::split(globalName1, ','); // in the case we have a list of globals (eg. .global x, y, z, etc.)

	for (auto& globalName : list) {
		symbol& s = ST->find(globalName);
		if (s.name == "NOT_FOUND")
			throw "Global " + globalName + " was not defined."; // we know cuz we were adding globals to symbol table during first pass
		else
			s.is_global = true; // if it is defined, we just set its is_global flag to true, even if it might have already been set.
	}
}

void Assembler::processExtern2(std::string line)
{
	// we do nothing when we pass extern during 2nd pass :)
}

void Assembler::processSkip2(std::string line)
{
	if (!line_analyzer::isSkip(line))return;
	int skip_value = line_analyzer::getLiteralFromSkip(line);
	this->location_counter += skip_value; // povecamo LC za argument .skip-a
	for (int i = 0; i < skip_value; i++) // a onda dodamo toliko 0x00 u byte code
		this->BC->addByte(this->current_section, 0);
}

void Assembler::processWord2(std::string line)
{
	if (!line_analyzer::isWord(line))return;

	if (line_analyzer::isWordInteger(line) || line_analyzer::isWordHex(line)) { // if word's argument is literal value
		int value = line_analyzer::getWordLiteral(line);
		BC->addWord(this->current_section, value); // we insert that value directly into code
	}
	else if (line_analyzer::isWordSymbol(line)) { // however, if it is a symbol, we will either insert the value if the symbol is in symbol table, or insert the symbol to relocation table
		symbol& s = ST->find(line_analyzer::getWordSymbolName(line));

		if (s.name == "NOT_FOUND") // if symbol doesn't exist in symbol table, it is error. Because all symbols need to be in symbol table after 1st pass
			throw "Word " + line_analyzer::getWordSymbolName(line) + " was not defined.";
		else { // if it is found
			if (s.section == "ABSOLUTE") 
				BC->addWord(this->current_section, s.value);
			else if (s.is_global) { 
				this->BC->addWord(this->current_section, 0); // add 0x00 code
				this->RT->add(this->current_section, "R_386_16", this->location_counter, s.id); // add reloc
			}
			else {
				this->BC->addWord(this->current_section, s.value); // add code
				this->RT->add(this->current_section, "R_386_16", this->location_counter, SL->getId(s.section));//add reloc
			}
		}
		this->location_counter += 2;
	}
}

void Assembler::processEnd2(std::string line)
{
	if (!line_analyzer::isEnd(line))return;
	end = true;
	this->current_section = "";
	this->location_counter = 0;
}

void Assembler::processEqu2(std::string line)
{
	// we do nothing when we pass equ during 2nd pass :)
}

void Assembler::processInstruction2(std::string line)
{
	if (!line_analyzer::isInstruction(line)) return;
	processJump2(line);
	processLdstr2(line);
	processInstructionTwoOperands(line);
	processInstructionOneOperand(line);
	processInstructionNoOperand(line);

	this->location_counter += line_analyzer::calculate_location_counter_for_instruction(line);
}

void Assembler::processLine2(std::string line)
{
	line = line_analyzer::removeComments(line);
	if (line_analyzer::isEmpty(line)) return;
	processLabel2(line);
	processEqu2(line);
	processExtern2(line);
	processSection2(line);
	processGlobal2(line);
	processWord2(line);
	processSkip2(line);
	processEnd2(line);
	processInstruction2(line);
}

void Assembler::processInstructionNoOperand(std::string line)
{
	if (line_analyzer::getInstructionName(line) == "halt" ||
		line_analyzer::getInstructionName(line) == "iret" ||
		line_analyzer::getInstructionName(line) == "ret")
		this->BC->addByte(this->current_section, line_analyzer::getInstructionCode(line));
}

void Assembler::processInstructionOneOperand(std::string line)
{
	if (!line_analyzer::isInstructionOneOperand(line)) return;
	//this->BC->addWord(this->current_section, this->location_counter);
	this->BC->addByte(this->current_section, line_analyzer::getInstructionCode(line));
	std::string instruction = line_analyzer::getInstructionName(line);
	if (instruction == "int" || instruction == "not") 
		this->BC->addByte(this->current_section, (line_analyzer::getRegisterCodeFirstArgument(line) << 4) + 15);
	if (instruction == "push" || instruction == "pop") 
		this->BC->addByte(this->current_section, (line_analyzer::getRegisterCodeFirstArgument(line) << 4) + 6);
	if (instruction == "pop") 
		this->BC->addByte(this->current_section, 0x42);
	if (instruction == "push") 
		this->BC->addByte(this->current_section, 0x12);

}

void Assembler::processInstructionTwoOperands(std::string line)
{
	if (!line_analyzer::isInstructionTwoOperands(line)) return;
	this->BC->addByte(this->current_section, line_analyzer::getInstructionCode(line)); // instr code

	int code = line_analyzer::getRegisterCodeFirstArgument(line);
	code <<= 4;
	code += line_analyzer::getRegisterCodeSecondArgument(line);

	this->BC->addByte(this->current_section, code); // code for registers
}

void Assembler::processJumpDirect2(std::string line)
{
	int value = 0;
	std::regex regex_jump_direct(regex_rules::regex_instruction_jmp_direct);
	std::smatch match;
	if (std::regex_match(line, match, regex_jump_direct))
		value = std::stoi(match.str(2), 0, 16); // catch literal
	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), 0xFF, 0, value);
}

void Assembler::processJumpRegDir2(std::string line)
{
	addThreeByteInstructionToBT(line_analyzer::getInstructionCode(line),
		line_analyzer::getRegisterCodeFirstArgument(line), 0x01); // instruction code, register code, 0x01
}

void Assembler::processJumpRegInd2(std::string line)
{
	addThreeByteInstructionToBT(line_analyzer::getInstructionCode(line),
		line_analyzer::getRegisterCodeFirstArgument(line), 0x02);// instruction code, register code, 0x02
}

void Assembler::processJumpPC2(std::string line)
{
	int value = 0;
	std::regex regex_jump_pc(regex_rules::regex_instruction_jmp_pc);
	std::smatch match;
	std::regex_match(line, match, regex_jump_pc);
	value = calculatePCRelSymbol(match.str(2)); // za drugi argument, sracunamo sta treba da stoji u kodu, u zavisnosti od operanda

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), 0xF7, 0x05, value);
}

void Assembler::processJumpAbsolute2(std::string line)
{
	int value = 0;
	std::regex regex_jump_abs2(regex_rules::regex_instruction_jmp_absolute);
	std::smatch match;
	std::regex_match(line, match, regex_jump_abs2);
	value = calculateAbsoluteSymbol(match.str(2));

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), 0xF0, 0, value);
}

void Assembler::processJumpMemDirSymbol2(std::string line)
{
	int value = 0;
	std::regex regex_jump_memdir(regex_rules::regex_instruction_jmp_memdir_symbol);
	std::smatch match;
	std::regex_match(line, match, regex_jump_memdir);
	value = calculateAbsoluteSymbol(match.str(2));

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), 0xff, 0x04, value);

}

void Assembler::processJumpMemDirLiteral2(std::string line)
{
	int value = 0;
	std::regex regex_jump_memdir(regex_rules::regex_instruction_jmp_memdir_literal);
	std::smatch match;
	if (std::regex_match(line, match, regex_jump_memdir))
		value = std::stoi(match.str(2), 0, 16);
	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), 0xff, 0x04, value);

}

void Assembler::processJump2(std::string line)
{
	if (line_analyzer::isJumpDirect(line)) processJumpDirect2(line);
	else if (line_analyzer::isJumpRegDir(line)) processJumpRegDir2(line);
	else if (line_analyzer::isJumpRegInd(line)) processJumpRegInd2(line);
	else if (line_analyzer::isJumpPC(line)) processJumpPC2(line);
	else if (line_analyzer::isJumpAbsolute(line)) processJumpAbsolute2(line);
	else if (line_analyzer::isJumpMemdirLiteral(line)) processJumpMemDirLiteral2(line);
	else if (line_analyzer::isJumpMemdirSymbol(line)) processJumpMemDirSymbol2(line);
}
void Assembler::processLdstrDirectLiteral(std::string line)
{
	int val = 0;
	std::regex regex_ldst_abs(regex_rules::regex_instruction_ldstr_direct_literal);
	std::smatch match;
	if (std::regex_match(line, match, regex_ldst_abs))
		val = std::stoi(match.str(3), 0, 16);

	int val_reg = line_analyzer::getRegisterCodeFirstArgument(line);
	int regs = (val_reg << 4) | 0x00;

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), regs, 0, val);
}

void Assembler::processLdstrDirectSymbol(std::string line) //TODO: reloc
{
	int value = 0;
	std::regex regex_ldst_abs2(regex_rules::regex_instruction_ldstr_direct_symbol);
	std::smatch match;
	regex_match(line, match, regex_ldst_abs2);
	value = calculateAbsoluteSymbol(match.str(3));

	int val_reg = line_analyzer::getRegisterCodeFirstArgument(line);
	int regs = (val_reg << 4) | 0x00;

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), regs, 0, value);
}

void Assembler::processLdstrRegDir2(std::string line)
{
	int reg_code = line_analyzer::getRegisterCodeFirstArgument(line);
	reg_code <<= 4;
	reg_code += line_analyzer::getRegisterCodeSecondArgument(line);

	addThreeByteInstructionToBT(line_analyzer::getInstructionCode(line), reg_code, 0x01); //TODO: 0x01
}

void Assembler::processLdstrRegInd2(std::string line)
{
	int reg_code = line_analyzer::getRegisterCodeFirstArgument(line);
	reg_code <<= 4;
	reg_code += line_analyzer::getRegisterCodeSecondArgument(line);

	addThreeByteInstructionToBT(line_analyzer::getInstructionCode(line), reg_code, 0x02); // TODO" 0x02
}

void Assembler::processLdstrMemdirAbsolute(std::string line)
{
	int value = 0;
	std::regex regex_ldst_memdir_absolute(regex_rules::regex_instruction_ldstr_memdir_absolute);
	std::smatch match;
	regex_match(line, match, regex_ldst_memdir_absolute);
	value = calculateAbsoluteSymbol(match.str(3));

	int val_reg = line_analyzer::getRegisterCodeFirstArgument(line);
	int regs = (val_reg << 4) | 0x00;

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), regs, 0, value); // TODO: change byte for addressing

}

void Assembler::processLdstrMemdirLiteral(std::string line)
{
	int val = 0;
	std::regex regex_ldst_memdir_literal(regex_rules::regex_instruction_ldstr_memdir_literal);
	std::smatch match;
	if (std::regex_match(line, match, regex_ldst_memdir_literal))
		val = std::stoi(match.str(3), 0, 16);

	int val_reg = line_analyzer::getRegisterCodeFirstArgument(line);
	int regs = (val_reg << 4) | 0x0f;

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), regs, 0, val); //TODO: zameni kod adresiranja
}

void Assembler::processLdstrPC2(std::string line)
{
	int value = 0;
	int reg_code = line_analyzer::getRegisterCodeFirstArgument(line);
	reg_code <<= 4;
	int regs = reg_code + 0x07;
	int adr = 0x03;
	std::regex regex_ldst_pc(regex_rules::regex_instruction_ldstr_pc);
	std::smatch match;
	std::regex_match(line, match, regex_ldst_pc);
	value = calculatePCRelSymbol(match.str(3));

	addFiveByteInstructionToBT(line_analyzer::getInstructionCode(line), reg_code, adr, value); // TODO" 0x02
}

void Assembler::processLdstr2(std::string line)
{
	if (line_analyzer::isLdstrRegDir(line)) 
		processLdstrRegDir2(line);
	else if (line_analyzer::isLdstrRegInd(line)) 
		processLdstrRegInd2(line);
	else if (line_analyzer::isLdstrDirectLiteral(line)) 
		processLdstrDirectLiteral(line);
	else if (line_analyzer::isLdstrDirectSymbol(line)) 
		processLdstrDirectSymbol(line);
	else if (line_analyzer::isLdstrPC(line)) 
		processLdstrPC2(line);
	else if (line_analyzer::isLdstrMemdirLiteral(line))
		processLdstrMemdirLiteral(line);
	else if (line_analyzer::isLdstrMemdirAbsolute(line)) 
		processLdstrMemdirAbsolute(line);
}




void Assembler::first_pass()
{
	if (!FR.reload()) exit(0);
	std::string line = "";
	while (line != "EOF") {
		line = FR.readln();
		if (line != "EOF") processLine1(line);
	}
}
void Assembler::save_symbol_table_in_file()
{
	this->ST->print_save();
}

void Assembler::print_symbol_table()
{
	this->ST->print();
}

void Assembler::second_pass()
{
	FR.reload();
	std::string line = ".";
	while (line != "EOF") {
		line = FR.readln();
		//std::cout << line << endl;
		if (line != "EOF") processLine2(line);
	}
}

void Assembler::print_code()
{
	this->BC->print();
}

void Assembler::print_code_with_addresses() 
{
	this->BC->print_hex();
}

void Assembler::save_code_in_file()
{
	this->BC->print_save();

}

void Assembler::save_relocation_tables_in_file()
{
	this->RT->print_save();
}

void Assembler::print_relocation_tables()
{
	this->RT->print();
}

void Assembler::send_all_to_linker()
{
	this->ST->send_to_linker();
	this->RT->send_to_linker();
	this->BC->send_to_linker();
	this->SL->send_to_linker();
}

void Assembler::processEnd1(std::string line) {
	if (!line_analyzer::isEnd(line))return;
	end = true;
	if (this->current_section != "")
		ST->find(this->current_section).size = this->location_counter;
	this->current_section = "";
	this->location_counter = 0;
}

void Assembler::addFiveByteInstructionToBT(int kod, int reg, int addrmode, int value)
{
	this->BC->addByte(this->current_section, kod);
	this->BC->addByte(this->current_section, reg);
	this->BC->addByte(this->current_section, addrmode);
	this->BC->addWord(this->current_section, value);
}

void Assembler::addThreeByteInstructionToBT(int instr, int reg, int addr)
{
	this->BC->addByte(this->current_section, instr);
	this->BC->addByte(this->current_section, reg);
	this->BC->addByte(this->current_section, addr);
}

int Assembler::calculateAbsoluteSymbol(std::string op)
{
	symbol& s = ST->find(op);
	if (s.name == "NOT_FOUND") throw "Failed trying to create absolute relocation entry for symbol " + s.name + ". Symbol not found in Symbol Table.";
	if (s.section == "ABSOLUTE") return s.value;
	if (s.is_global || s.section == "") {
		RT->add(this->current_section, "R_386_16", this->location_counter + 3, s.id);
		return 0;
	}
	else {
		RT->add(this->current_section, "R_386_16", this->location_counter + 3, SL->getId(s.section));
		return s.value;
	}
}

int Assembler::calculatePCRelSymbol(std::string op)
{
	symbol& s = this->ST->find(op);
	int val = 0;
	if (s.name == "NOT_FOUND")
		throw "Failed trying to create PC-relative relocation entry for symbol " + op + ". Symbol not found in Symbol Table.";

	if (s.is_global || s.section == "") {
		if (SL->getId(s.section) == ST->find(this->current_section).id) {
			return s.value - 2 - this->location_counter - 3;
		}
		else {
			RT->add(this->current_section, "R_386_PC16", this->location_counter + 3, s.id);
			return -2;
		}
	}
	else {
		if (SL->getId(s.section) == SL->getId(this->current_section)) {
			return s.value - 2 - location_counter - 3;
		}
		else {
			RT->add(this->current_section, "R_386_PC16", this->location_counter + 3, SL->getId(s.section));
			return s.value - 2;
		}
	}
}

