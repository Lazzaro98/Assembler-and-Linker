#include "regex_rules.h"

std::string regex_rules::regex_empty = "^\\s*$";
std::string regex_rules::regex_label = "^[ \\t]*([a-zA-Z][a-zA-Z0-9_]*)[:][ \t]*$";
std::string regex_rules::regex_section = "\\s*.section\\s+([a-zA-Z_][a-zA-Z_0-9]*)\\s*";
std::string regex_rules::regex_equ = "^\\s*[.]equ\\s+([a-zA-Z_][a-zA-Z_0-9]*)\\s*,\\s*((?:[a-zA-Z_][a-zA-Z_0-9]*|(?:[+]|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})(?:\\s*(?:[+]|-)\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4}|[(](?:[+]|-)[0-9]+[)]))*\\s*)\\s*$";
std::string regex_rules::regex_global = "^\\s*\\.global\\s+((?:\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*,)*\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*)\\s*$";
std::string regex_rules::regex_extern = "^\\s*\\.extern\\s+((?:\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*,)*\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*)\\s*$";
std::string regex_rules::regex_word = "^\\s*\\.word\\s+((?:\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|(?:\\+|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})\\s*,)*\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|(?:\\+|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})\\s*)\\s*$";
std::string regex_rules::regex_word_integer = "^[ \\t]*[.]word[ \t]*([0-9]*)[ \\t]*$";
std::string regex_rules::regex_word_hex = "^[ \\t]*[.]word[ \t]*0[xX]([1-9a-fA-F][a-fA-F0-9]{0,3})[ \\t]*$";
std::string regex_rules::regex_word_symbol = "^[ \\t]*[.]word[ \t]*([a-zA-Z_][a-z0-9_]*)[ \\t]*$";
std::string regex_rules::regex_skip = "^[ \\t]*.skip[ ]([a-zA-Z0-9_,]*)[ \\t]*$";
std::string regex_rules::regex_end = "^\\s*\\.end\\s*$";



std::string regex_rules::regex_instruction_name = "^\\s*([A-Za-z]*).*";

std::string regex_rules::regex_instruction_no_operand = "^\\s*(halt|iret|ret)\\s*$";
std::string regex_rules::regex_instruction_one_operand = "^[ \\t]*(push|pop|int|not)[ \\t]*(r[0-7]|psw|sp|pc)$";// kod one operand instrukcija u postavci je operand uvek registar. 
// Kada imamo 2 operanda, uvek ce oba biti registri osim kod LDR/STR. Te dve instrukcije cemo svakako obraditi posebno, tako da cemo ovde smatrati da su oba operanda registri.
std::string regex_rules::regex_instruction_two_operands = "^[ \\t]*(add|sub|mul|div|cmp|and|or|xor|shl|shr|test|xchg) [ \\t]*(r[0-7]|psw|sp|pc)[ \\t]*,[ \\t]*(r[0-7]|psw|sp|pc)$";


//jumps
std::string regex_rules::regex_instruction_jmp_direct = "[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})[ \\t]*$";
std::string regex_rules::regex_instruction_jmp_regdir = "^[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*[*](r[0-7]|psw|sp|pc)[ \\t]*$";
std::string regex_rules::regex_instruction_jmp_regind = "^[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*[*]\\[(r[0-7]|psw|sp|pc)\\][ \\t]*$";
std::string regex_rules::regex_instruction_jmp_pc = "^[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*[%]([a-zA-Z][0-9a-zA-Z]*)[ \\t]*$";
std::string regex_rules::regex_instruction_jmp_absolute = "^[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*([a-zA-Z][0-9a-zA-Z]*)[ \\t]*$";
std::string regex_rules::regex_instruction_jmp_memdir_symbol = "^[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*[*]([a-zA-Z][0-9a-zA-Z]*)[ \\t]*$";
std::string regex_rules::regex_instruction_jmp_memdir_literal = "^[ \\t]*(jmp|call|jeq|jne|jgt)[ \\t]*[*]0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})[ \\t]*$";

//ldr/str
std::string regex_rules::regex_instruction_ldstr_direct_literal = "^[ \\t]*(ldr|str)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*[$]0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})[ \\t]*$";
std::string regex_rules::regex_instruction_ldstr_direct_symbol = "^[ \\t]*(ldr|str)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*[$]([a-zA-Z][a-zA-Z0-9_]*)[ \\t]*$";
std::string regex_rules::regex_instruction_ldstr_regdir = "^[ \\t]*(ldr|str)[ \\t]*(r[0-7]|psw|sp|pc)[,][ \\t]*[*](r[0-7]|psw|sp|pc)[ \\t]*$";
std::string regex_rules::regex_instruction_ldstr_regind = "^[ \\t]*(ldr|str)[ \\t]*(r[0-7]|psw|sp|pc)[,][ \\t]*\\[(r[0-7]|psw|sp|pc)\\][ \\t]*$";
std::string regex_rules::regex_instruction_ldstr_memdir_absolute = "^[ \\t]*(ldr|str)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*([a-zA-Z][a-zA-Z0-9_]*)[ \\t]*$";
std::string regex_rules::regex_instruction_ldstr_memdir_literal = "^[ \\t]*(ldr|str)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})[ \\t]*$";
std::string regex_rules::regex_instruction_ldstr_pc = "^[ \\t]*(ldr|str)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*[%]([a-zA-Z][a-zA-Z0-9_]*)[ \\t]*$";



std::string regex_rules::regex_catch_first_register = "^[ \\t]*([a-zA-Z]*)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*.*$";
std::string regex_rules::regex_catch_second_register = "^[ \\t]*([a-zA-Z]*)[ \\t](r[0-7]|psw|pc|sp)[,][ \\t]*(r[0-7]|psw|pc|sp)$";



//linker
std::string regex_rules::regex_linker_arguments = "[ \\t]*-place[ \\t]*=([a-zA-Z0-9_]*)@0[xX]{0,1}([0-9][0-9a-fA-F]{0,3})[ \\t]*";