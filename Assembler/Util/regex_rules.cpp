#include "regex_rules.h"

std::string regex_rules::regex_empty = "^\\s*$";
std::string regex_rules::regex_label = "\s*([a-zA-Z][a-zA-Z0-9_]*)[:]\s*";
std::string regex_rules::regex_section = "\\s*.section\\s+([a-zA-Z_][a-zA-Z_0-9]*)\\s*";
std::string regex_rules::regex_equ = "^\\s*[.]equ\\s+([a-zA-Z_][a-zA-Z_0-9]*)\\s*,\\s*((?:[a-zA-Z_][a-zA-Z_0-9]*|(?:[+]|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})(?:\\s*(?:[+]|-)\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4}|[(](?:[+]|-)[0-9]+[)]))*\\s*)\\s*$";
std::string regex_rules::regex_global = "^\\s*\\.global\\s+((?:\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*,)*\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*)\\s*$";
std::string regex_rules::regex_extern = "^\\s*\\.extern\\s+((?:\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*,)*\\s*[a-zA-Z_][a-zA-Z_0-9]*\\s*)\\s*$";
std::string regex_rules::regex_word = "^\\s*\\.word\\s+((?:\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|(?:\\+|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})\\s*,)*\\s*(?:[a-zA-Z_][a-zA-Z_0-9]*|(?:\\+|-)?[0-9]+|(?:0x|0X)[0-9a-fA-F]{1,4})\\s*)\\s*$";
std::string regex_rules::regex_word_integer = "^\\s*[.]word[ \t]*([0-9]*)\\s*$";
std::string regex_rules::regex_word_hex = "^\\s*[.]word[ \t]*0[xX]([1-9a-fA-F][a-fA-F0-9]{0,3})\\s*$";
std::string regex_rules::regex_word_symbol = "^\\s*[.]word[ \t]*([a-zA-Z_][a-z0-9_]*)\\s*$";
std::string regex_rules::regex_skip = "^\\s*.skip[ ]([a-zA-Z0-9_,]*)\\s*$";
std::string regex_rules::regex_end = "^\\s*\\.end\\s*$";



std::string regex_rules::regex_instruction_name = "^\\s*([A-Za-z]*)\\s*";

std::string regex_rules::regex_instruction_no_operand = "^\\s*(halt|iret|ret)\\s*$";
std::string regex_rules::regex_instruction_one_operand = "^\\s*(push|pop|int|not)\\s*(r[0-7]|psw|sp|pc)$";// kod one operand instrukcija u postavci je operand uvek registar. 
// Kada imamo 2 operanda, uvek ce oba biti registri osim kod LDR/STR. Te dve instrukcije cemo svakako obraditi posebno, tako da cemo ovde smatrati da su oba operanda registri.
std::string regex_rules::regex_instruction_two_operands = "^\\s*(add|sub|mul|div|cmp|and|or|xor|shl|shr|test|xchg) \\s*(r[0-7]|psw|sp|pc)\\s*,\\s*(r[0-7]|psw|sp|pc)$";


//jumps
std::string regex_rules::regex_instruction_jmp_direct = "\\s*(jmp|call|jeq|jne|jgt)\\s*0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})\\s*$";
std::string regex_rules::regex_instruction_jmp_regdir = "^\\s*(jmp|call|jeq|jne|jgt)\\s*[*](r[0-7]|psw|sp|pc)\\s*$";
std::string regex_rules::regex_instruction_jmp_regind = "^\\s*(jmp|call|jeq|jne|jgt)\\s*[*]\\[(r[0-7]|psw|sp|pc)\\]\\s*$";
std::string regex_rules::regex_instruction_jmp_pc = "^\\s*(jmp|call|jeq|jne|jgt)\\s*[%]([a-zA-Z][0-9a-zA-Z]*)\\s*$";
std::string regex_rules::regex_instruction_jmp_absolute = "^\\s*(jmp|call|jeq|jne|jgt)\\s*([a-zA-Z][0-9a-zA-Z]*)\\s*$";
std::string regex_rules::regex_instruction_jmp_memdir_symbol = "^\\s*(jmp|call|jeq|jne|jgt)\\s*[*]([a-zA-Z][0-9a-zA-Z]*)\\s*$";
std::string regex_rules::regex_instruction_jmp_memdir_literal = "^\\s*(jmp|call|jeq|jne|jgt)\\s*[*]0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})\\s*$";
std::string regex_rules::regex_instruction_jmp_displ_literal = "\\s*(jmp|call|jeq|jne|jgt)\\s* \\s*[*]\\[(r[0-8])\\s*[+]\\s*0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})\\]$"; //group1->instr, group2->registar, group3->brojna vrednost bez 0x
std::string regex_rules::regex_instruction_jmp_displ_symbol = "^\\s*(jmp|call|jeq|jne|jgt)\\s*\\s*[*]\\[(r[0-8])\\s*[+]\\s*([a-zA-Z_][a-zA-Z_]*)\\]$";// group1 -> jump, group2->registar, group3->simbol
//ldr/str
std::string regex_rules::regex_instruction_ldstr_direct_literal = "^\\s*(ldr|str)\\s(r[0-7]|psw|pc|sp)[,]\\s*[$]0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})\\s*$";
std::string regex_rules::regex_instruction_ldstr_direct_symbol = "^\\s*(ldr|str)\\s(r[0-7]|psw|pc|sp)[,]\\s*[$]([a-zA-Z][a-zA-Z0-9_]*)\\s*$";
std::string regex_rules::regex_instruction_ldstr_regdir = "^\\s*(ldr|str)\\s*(r[0-7]|psw|sp|pc)[,]\\s*[*](r[0-7]|psw|sp|pc)\\s*$";
std::string regex_rules::regex_instruction_ldstr_regind = "^\\s*(ldr|str)\\s*(r[0-7]|psw|sp|pc)[,]\\s*\\[(r[0-7]|psw|sp|pc)\\]\\s*$";
std::string regex_rules::regex_instruction_ldstr_memdir_absolute = "^\\s*(ldr|str)\\s(r[0-7]|psw|pc|sp)[,]\\s*([a-zA-Z][a-zA-Z0-9_]*)\\s*$";
std::string regex_rules::regex_instruction_ldstr_memdir_literal = "^\\s*(ldr|str)\\s(r[0-7]|psw|pc|sp)[,]\\s*0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})\\s*$";
std::string regex_rules::regex_instruction_ldstr_pc = "^\\s*(ldr|str)\\s(r[0-7]|psw|pc|sp)[,]\\s*[%]([a-zA-Z][a-zA-Z0-9_]*)\\s*$";
std::string regex_rules::regex_instruction_ldstr_displ_literal = "\\s*(ldr|str)\\s*(r[0-7]|psw|pc|sp)\\s*[,]\\s*\\[(r[0-7]|psw|pc|sp)\\s*[+]\\s*0{0,1}[xX]{0,1}([1-9][0-9a-fA-F]{0,3})\\][\\t ]*$";// group1 ->instr, group2->registar, group3->brojna vrednost bez 0x
std::string regex_rules::regex_instruction_ldstr_displ_symbol = "\\s*(ldr|str)\\s*(r[0-7]|psw|pc|sp)\\s*[,]\\s*\\[(r[0-7]|psw|pc|sp)\\s*[+]\\s*([a-zA-Z_][a-zA-Z0-9)]*)\\][\\t ]*$";

std::string regex_rules::regex_catch_first_register = "^\\s*([a-zA-Z]*)\\s(r[0-7]|psw|pc|sp)\\s*.*$";
std::string regex_rules::regex_catch_second_register = "^\\s*([a-zA-Z]*)\\s(r[0-7]|psw|pc|sp)[,]\\s*(r[0-7]|psw|pc|sp)$";



//linker
std::string regex_rules::regex_linker_arguments = "\\s*-place\\s*=([a-zA-Z0-9_]*)@0[xX]{0,1}([0-9][0-9a-fA-F]{0,3})\\s*";