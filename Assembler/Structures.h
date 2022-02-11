#ifndef _Structures_h_
#define _Structures_h_
#include <iostream>
#include<fstream>
#include <map>
#include <list>
#include <iomanip>
#include <sstream>
#include "Variadic_table.h"

#define FILE_FOR_LINKER "from_linker.o"

struct symbol {
	unsigned int id;
	int value;
	int size;
	bool is_global;
	bool is_defined;
	std::string name;
	std::string section;
	int pomeraj;
	//bool is_extern;

	symbol(std::string name, std::string section, int value, int size, bool is_global, int id, bool is_defined = true)
	{
		this->name = name;
		this->section = section;
		this->value = value;
		this->size = size;
		this->is_global = is_global;
		this->id = id;
		//this->is_extern = is_extern;
		this->is_defined = is_defined;
		pomeraj = 0;
	}
	symbol() {}

	bool operator<(const symbol& rhs) const noexcept
	{
		// logic here
		return this->name < rhs.name; // for example
	}
};

struct rtentry {
	int offset;
	std::string type;
	int value;
	std::string section;
	rtentry(std::string type, int offset, int value) {
		this->offset = offset;
		this->type = type;
		this->value = value;
	}
};



class SymbolTable {
	std::map<std::string, symbol> symbol_table;

	int id_s = 0;
	symbol undefinedSymbol{ "NOT_FOUND", "NOT_FOUND", 0, 0, 0, 0 };

public:

	SymbolTable() {
		add("UNDEFINED", "UNDEFINED", 0, 0, false, true);
		add("ABSOLUTE", "ABSOLUTE", 0, 0, false, true);
	}

	void add(symbol s) {
		if (&s == nullptr) return;
		symbol_table[s.name] = s;
		if (s.id > id_s) id_s = s.id + 1;
	}

	void add(std::string name, std::string section, int value, int size, bool is_global, int id, bool is_defined = true) {
		symbol s(name, section, value, size, is_global, id, is_defined);
		symbol_table[name] = s;
		if (id > id_s) id_s = id + 1; // da pripremimo ID za dodavanje sledeceg, ako bude uopste trebalo.
	}

	void add(std::string name, std::string section, int value, int size, bool is_global, bool is_defined = true) {
		symbol s(name, section, value, size, is_global, id_s++, is_defined);
		symbol_table[name] = s;
	}

	symbol& find(std::string name) {
		std::map<std::string, symbol>::iterator it = symbol_table.find(name);
		bool exists = !(it == symbol_table.end());
		if (exists) return it->second;
		else return undefinedSymbol;
	}

	bool exists(std::string name) {
		std::map<std::string, symbol>::iterator it = symbol_table.find(name);
		bool exists = !(it == symbol_table.end());
		return exists;
	}

	std::list<symbol> get_sections() {
		std::list<symbol> sections;
		for (auto& entry : symbol_table) {
			if (entry.second.section == entry.second.name)
				sections.push_back(entry.second);
		}
		return sections;
	}

	std::list<symbol> get_globals() {
		std::list<symbol> globals;
		for (auto& entry : symbol_table) {
			if (entry.second.is_global && entry.second.section != "UNDEFINED")
				globals.push_back(entry.second);
		}
		return globals;
	}

	std::list<symbol> get_externs() {
		std::list<symbol> externs;
		for (auto& entry : symbol_table) {
			if (entry.second.section == "UNDEFINED")
				externs.push_back(entry.second);
		}
		return externs;
	}


	void print() {
		std::cout << "Symbol table:" << std::endl;
		VariadicTable<std::string, int, std::string, std::string, int, int> vt({ "Symbol", "Value", "Type", "Section", "Id", "Size" },
			10);
		for (auto& entry : symbol_table) {
			std::string type = "g";
			if (entry.second.is_global == false) type = "l";
			vt.addRow(entry.second.name, entry.second.value, type, entry.second.section, entry.second.id, entry.second.size);
		}

		vt.print(std::cout);
	}

	void print_save(std::string filename = "output.txt") {
		std::ofstream file(filename);
		VariadicTable<std::string, int, std::string, std::string, int, int> vt({ "Symbol", "Value", "Type", "Section", "Id", "Size" },
			10);
		for (auto& entry : symbol_table) {
			std::string type = "g";
			if (entry.second.is_global == false) type = "l";
			vt.addRow(entry.second.name, entry.second.value, type, entry.second.section, entry.second.id, entry.second.size);
		}

		vt.print(file);
		file << std::dec << std::endl;
		file.close();
	}

	void send_to_linker()
	{
		std::ofstream file(FILE_FOR_LINKER);
		for (auto& entry : symbol_table) {
			std::string type = "g";
			if (entry.second.is_global == false) type = "l";
			file << entry.second.name << "," << entry.second.value << "," << type << "," << entry.second.section << "," << entry.second.id << "," << entry.second.size << std::endl;
		}
		file << "STEND" << std::endl;
		file << std::dec << std::endl;
		file.close();
	}
};
//int SymbolTable::id_s1 = 0;


class BinaryCode
{
private:
	std::map<std::string, std::vector<char> > relocation_table;
	std::map<std::string, int> start_addresses;
public:
	void addByte(std::string section, char byte)
	{
		this->relocation_table[section].push_back(byte);
	}
	void addWord(std::string section, int wordValue)
	{
		this->relocation_table[section].push_back((((unsigned)wordValue) >> 8) & 0xFF);
		this->relocation_table[section].push_back(((unsigned)wordValue) & 0xFF);
	}
	void addWord(std::string section, std::string word)
	{
		this->addWord(section, std::stoi(word));
	}
	void addByteArray(std::string section, std::vector<char> arr) {
		for (char c : arr) {
			addByte(section, c);
		}
	}
	std::vector<char> get_code_by_section(std::string section) {
		return this->relocation_table[section];
	}
	void clear()
	{
		this->relocation_table.clear();
	}
	void print() {
		for (const auto& elem : this->relocation_table)
		{
			std::cout << elem.first << " -> ";
			for (char i : elem.second)
				std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)i) & 0xFF) << " ";
			std::cout << std::endl;
		}
	}
	void print_hex() {
		int cnt = 0;
		for (const auto& elem : this->relocation_table)
		{
			std::cout << elem.first << ":";
			cnt = this->start_addresses[elem.first];

			while (cnt % 8 != 0) cnt--;

			if (cnt < this->start_addresses[elem.first]) {
				std::cout << std::endl;
				std::cout << std::hex << std::setw(4) << std::setfill('0') << (((int)cnt) & 0xFFFF) << ": ";
			}

			while (cnt < this->start_addresses[elem.first]) {
				std::cout << std::hex << std::setw(2) << std::setfill('0') << (((int)0) & 0xFF) << " ";
				cnt++;
			}


			for (char i : elem.second) {
				if (cnt % 8 == 0) {
					std::cout << std::endl;
					std::cout << std::hex << std::setw(4) << std::setfill('0') << (((int)cnt) & 0xFFFF) << ": ";
				}
				std::cout << std::hex << std::setw(2) << std::setfill('0') << (((int)i) & 0xFF) << " ";
				cnt++;
			}
			std::cout << std::endl;
		}
	}

	void print_hex_save(std::string filename = "output.txt") {
		std::ofstream file(filename);
		int cnt = 0;
		for (const auto& elem : this->relocation_table)
		{
			file << elem.first << ":";
			cnt = this->start_addresses[elem.first];

			while (cnt % 8 != 0) cnt--;

			if (cnt < this->start_addresses[elem.first]) {
				file << std::endl;
				file << std::hex << std::setw(4) << std::setfill('0') << (((int)cnt) & 0xFFFF) << ": ";
			}

			while (cnt < this->start_addresses[elem.first]) {
				file << std::hex << std::setw(2) << std::setfill('0') << (((int)0) & 0xFFFF) << " ";
				cnt++;
			}


			for (char i : elem.second) {
				if (cnt % 8 == 0) {
					file << std::endl;
					file << std::hex << std::setw(4) << std::setfill('0') << (((int)cnt) & 0xFFFF) << ": ";
				}
				file << std::hex << std::setw(2) << std::setfill('0') << (((int)i) & 0xFF) << " ";
				cnt++;
			}
			file << std::endl;
		}
	}
	void print_save(std::string filename = "output.txt") {
		std::ofstream file(filename, std::ios_base::app);
		for (const auto& elem : this->relocation_table)
		{
			file << elem.first << "\n";
			for (char i : elem.second)
				file << "0x" << std::hex << std::setw(2) << std::setfill('0') << (((int)i) & 0xFF) << " ";
			file << std::endl << std::endl;
		}
	}

	void send_to_linker()
	{
		std::ofstream file(FILE_FOR_LINKER, std::ios_base::app);
		for (const auto& elem : this->relocation_table)
		{
			file << elem.first << "\n";
			for (char i : elem.second)
				file << (((int)i) & 0xFF) << ",";
			file << std::endl;
		}
		file << "BCEND" << std::endl;
		file << std::endl;
	}

	void load_start_addresses(std::map<std::string, int> m) {
		this->start_addresses = m;
	}
	// pomocna f-ja
	template< typename T >
	std::string int_to_hex(T i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(T) * 2)
			<< std::hex << i;
		return stream.str();
	}
};

class RelocationTable {
	std::map<std::string, std::list<rtentry>> relocation_table;
public:
	RelocationTable()
	{
	}
	void add(std::string section, rtentry rt)
	{
		this->relocation_table[section].push_back(rt);
	}
	void add(std::string section, std::string type, int offset, int value)
	{
		this->add(section, rtentry(type, offset, value));
	}
	void print()
	{

		for (auto& entry : relocation_table)
		{//za svaku tabelu
			VariadicTable<std::string, int, int> vt({ "Type", "Offset", "Value" },
				10);
			for (rtentry rentry : entry.second) {
				//std::cout << rentry.type << "   " << rentry.offset << "   " << rentry.value << endl;
				vt.addRow(rentry.type, rentry.offset, rentry.value);
			}
			std::cout << entry.first << std::endl;
			vt.print(std::cout);
		}
	}
	void print_save(std::string output_filename = "output.txt")
	{
		std::ofstream file(output_filename, std::ios_base::app);
		//file.open();
		for (auto& entry : relocation_table)
		{//za svaku tabelu
			VariadicTable<std::string, int, int> vt({ "Type", "Offset", "Value" },
				10);
			//std::cout << entry.first<<endl;
			for (rtentry rentry : entry.second) {
				//std::cout << rentry.type << "   " << rentry.offset << "   " << rentry.value << endl;
				vt.addRow(rentry.type, rentry.offset, rentry.value);
			}
			file << entry.first << '\n';
			vt.print(file);
		}
		file.close();
	}

	void send_to_linker()
	{
		std::ofstream file(FILE_FOR_LINKER, std::ios_base::app);
		//file.open();
		for (auto& entry : relocation_table)
		{//za svaku tabelu
			//std::cout << entry.first<<endl;
			file << entry.first << '\n';
			for (rtentry rentry : entry.second) {
				//std::cout << rentry.type << "   " << rentry.offset << "   " << rentry.value << endl;
				file << rentry.type << "," << rentry.offset << "," << rentry.value << std::endl;
			}
			file << std::endl;
		}
		file << "RTEND" << std::endl;
		file << std::endl;

		file.close();
	}

	void clear()
	{
		this->relocation_table.clear();
	}
};

struct section
{
	std::string name;
	int id;
	section(std::string s, int i)
	{
		this->name = s;
		this->id = i;
	}
};
class SectionList
{
	std::list<section> section_list;
public:

	SectionList()
	{
		add("UNDEFINED", 0);
		add("ABSOLUTE", 1);
	}
	void add(std::string name, int id)
	{
		section_list.push_back(section(name, id));
	}
	void add(section s)
	{
		section_list.push_back(s);
	}
	int getId(std::string name)
	{
		for (auto& s : section_list)
		{
			if (s.name == name)
				return s.id;
		}
		return -1;
	}
	void send_to_linker() {
		std::ofstream file(FILE_FOR_LINKER, std::ios_base::app);
		for (auto& s : section_list)
			file << s.name << "," << s.id << std::endl;
		file << "SLEND" << std::endl;
	}
};


#endif