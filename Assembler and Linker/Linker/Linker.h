#ifndef _LINKER_H_
#define _LINKER_H_
#include <iostream>
#include <map>
#include <list>
#include <set>
#include "../Util/File.h"
class Linker
{

	void load_sections();
	void load_globals();
	void check_externs();
	void load_reloc();
	void resolve_start_addresses();
	bool is_overlap(std::string sectionName);
	void merge_code();
	int ids = 2;

	std::string mode = "";
	std::map<std::string, int> input_start_addresses;
	std::map<std::string, int> section_start_addresses;
	std::map<std::string, int> section_movement;
	std::list<std::string> input_files;
	std::string output_filename;

	std::map<std::string, File> loaded_files;

	//linker's info
	SymbolTable* ST;
	BinaryCode* BC;
	RelocationTable* RT;
	SectionList* SL;

public:
	Linker(int argc, char* argv[]) {
		ST = new SymbolTable();
		BC = new BinaryCode();
		RT = new RelocationTable();
		SL = new SectionList();
		load_arguments(argc, argv);
	}
	void load_arguments(int argc, char* argv[]); // just loads arguments from the args
	void receive_from_assembler(); // loads information about assemble-ed code
	void output();

	// private functions, but public now for testing
	
	void link();

	/*void save_code_in_file();
	void save_relocation_tables_in_file();
	void print_relocation_tables();*/
	//output
	void print_symbol_table();
	void print_relocation_table();
	void print_code();
	void print_hex();
	void save_symbol_table_in_file();
	void save_code_in_file();
	void save_relocation_tables_in_file();
	void save_code_hex_in_file();

	void delete_ol_files();

};
//testing git
#endif // !_LINKER_H_




