#ifndef _regex_rules_h_
#define _regex_rules_h_
#include <regex>
#include<iostream>
class regex_rules
{
public:
	static std::string regex_empty;
	static std::string regex_label;
	static std::string regex_section;
	static std::string regex_equ;
	static std::string regex_global;
	static std::string regex_extern;
	static std::string regex_word;
	static std::string regex_word_integer;
	static std::string regex_word_hex;
	static std::string regex_word_symbol;
	static std::string regex_skip;
	static std::string regex_end;


	static std::string regex_instruction_name;// regex that we use to grab instruction name

	static std::string regex_instruction_no_operand;
	static std::string regex_instruction_one_operand;
	static std::string regex_instruction_two_operands;

	//all types of jumps
	static std::string regex_instruction_jmp_direct;
	static std::string regex_instruction_jmp_regdir;
	static std::string regex_instruction_jmp_regind;
	static std::string regex_instruction_jmp_pc;
	static std::string regex_instruction_jmp_absolute;
	static std::string regex_instruction_jmp_memdir_symbol;
	static std::string regex_instruction_jmp_memdir_literal;

	//all types of ld/strs
	static std::string regex_instruction_ldstr_direct_literal;
	static std::string regex_instruction_ldstr_direct_symbol;
	static std::string regex_instruction_ldstr_regdir;
	static std::string regex_instruction_ldstr_regind;
	static std::string regex_instruction_ldstr_memdir_absolute;
	static std::string regex_instruction_ldstr_memdir_literal;
	static std::string regex_instruction_ldstr_pc;

	// regex to capture first argument which is register in any instruction
	static std::string regex_catch_first_register;
	static std::string regex_catch_second_register;

	//linker
	static std::string regex_linker_arguments;
};
#endif
