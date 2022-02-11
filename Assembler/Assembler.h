#ifndef _Assembler_h_
#define _Assembler_h_

#include <iostream>
#include <algorithm>
#include "regex_rules.h"
#include "Structures.h"
#include "line_analyzer.h"
#include "FileReader.h"

class Assembler
{
private:
	// first pass (1 represents first pass)
	void processLabel1(std::string label);
	void processSection1(std::string line);
	void processGlobal1(std::string line);
	void processExtern1(std::string line);
	void processSkip1(std::string line);
	void processWord1(std::string line);
	void processEqu1(std::string line);
	void processEnd1(std::string line);
	void processInstruction1(std::string line);
	void processLine1(std::string line);


	// second pass (2 represents second pass)
	void processLabel2(std::string label);
	void processSection2(std::string line);
	void processGlobal2(std::string line);
	void processExtern2(std::string line);
	void processSkip2(std::string line);
	void processWord2(std::string line);
	void processEnd2(std::string line);
	void processEqu2(std::string line);
	void processInstruction2(std::string line);
	void processLine2(std::string line);


	// process instruction methods

	void processInstructionNoOperand(std::string line);
	void processInstructionOneOperand(std::string line);
	void processInstructionTwoOperands(std::string line);

	//helper func ldr str
	void processJumpDirect2(std::string line); //  <literal> - vrednost <literal>
	void processJumpRegDir2(std::string line); // *<reg> - vrednost u registru <reg>
	void processJumpRegInd2(std::string line); // *[<reg>] - vrednost iz memorije na adresi <reg>
	void processJumpPC2(std::string line); // %<simbol> - vrednost <simbol> PC relativnim adresiranjem 
	void processJumpAbsolute2(std::string line); // <simbol> - vrednost <simbol> apsolutnim adresiranjem
	void processJumpMemDirSymbol2(std::string line); // *<literal> - vrednost iz memorije na adresi <literal>
	void processJumpMemDirLiteral2(std::string line); // *<literal> - vrednost iz memorije na adresi <literal>
	//final process jump
	void processJump2(std::string line);

	//help func process ldr str
	void processLdstrDirectLiteral(std::string line); //  $<literal> - vrednost <literal>
	void processLdstrDirectSymbol(std::string line); // $<simbol> - vrednost <simbol>
	void processLdstrRegDir2(std::string line);// <reg> - vrednost u registru <reg>
	void processLdstrRegInd2(std::string line);// [<reg>] - vrednost iz memorije na adresi <reg>
	void processLdstrMemdirAbsolute(std::string line); // <simbol> - vrednost iz memorije na adresi <simbol> apsolutnim adresiranjem
	void processLdstrMemdirLiteral(std::string line);// <literal> - vrednost iz memorije na adresi <literal>
	void processLdstrPC2(std::string line);// %<simbol> - vrednost iz memorije na adresi <simbol> PC relativnim adresiranjem
	//final process ldr str
	void processLdstr2(std::string line);

	// forming code
	void addFiveByteInstructionToBT(int kod, int reg, int addrmode, int value);
	void addThreeByteInstructionToBT(int kod, int reg, int addr);


	int calculateAbsoluteSymbol(std::string op);
	int calculatePCRelSymbol(std::string op);


public:
	int location_counter = 0;
	std::string current_section = "";
	bool end = false;
	std::string input_file_name, output_file_name;

	SymbolTable* ST = new SymbolTable();
	FileReader FR = FileReader();
	BinaryCode* BC = new BinaryCode();
	RelocationTable* RT = new RelocationTable();
	SectionList* SL = new SectionList();

	// first pass public methods
	void first_pass();
	void save_symbol_table_in_file();
	void print_symbol_table();
	void second_pass();
	void print_code();
	void save_code_in_file();
	void save_relocation_tables_in_file();
	void print_relocation_tables();
	void print_code_with_addresses();
	void send_all_to_linker();

	Assembler(std::string input_file_name = "ulaz.txt", std::string output_file_name = "izlaz.txt")
	{
		this->input_file_name = input_file_name;
		this->output_file_name = output_file_name;
		FR.set_input_filename(input_file_name);
	}
};

#endif