#ifndef  _FILE_H_
#define _FILE_H_

#include "Structures.h"
#include "FileReader.h"
#include "line_analyzer.h"
class File
{
	std::string filename;
public:
	SymbolTable* ST;
	FileReader* FR;
	BinaryCode* BC;
	RelocationTable* RT;
	SectionList* SL;
	File(std::string file_name = "input.o") {
		ST = new SymbolTable();
		FR = new FileReader(file_name);
		BC = new BinaryCode();
		RT = new RelocationTable();
		SL = new SectionList();
	}

	void receive_from_assembler();
	void set_filename(std::string filename) { this->filename = filename; }
	std::string get_filename() { return this->filename; }

};
#endif // ! _FILE_H_



