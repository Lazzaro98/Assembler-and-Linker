#include "../Util/File.h"



void File::receive_from_assembler()
{
	//prvo ucitavamo Symbol Table
		//void add(std::string name, std::string section, int value, int size, bool is_global, bool is_defined = true)

	if (!FR->reload()) throw "Couldn't reload file " + FR->get_input_filename() + ".Try reopening it.";
	std::string line = FR->readln();;
	while (line != "STEND") {
		std::vector<std::string> list = line_analyzer::split(line, ',');
		std::string name = list[0];
		int value = stoi(list[1]);
		bool is_global = false;
		if (list[2] == "g") is_global = true;
		std::string section = list[3];
		int id = stoi(list[4]);
		int size = stoi(list[5]);
		ST->add(name, section, value, size, is_global, id);
		line = FR->readln();
	}
	//symbol table received
	//now to receive RelocationTable
	while ((line = this->FR->readln()) != "RTEND") {
		if (line_analyzer::isEmpty(line)) continue;
		std::string section = line;
		line = this->FR->readln();
		while (!line_analyzer::isEmpty(line)) {
			std::vector<std::string> list = line_analyzer::split(line, ',');
			std::string type = list[0];
			int offset = stoi(list[1]);
			int value = stoi(list[2]);
			this->RT->add(section, type, offset, value);
			line = this->FR->readln();
		}
	}

	// Relocation table read
	// now time for binary code
	while ((line = this->FR->readln()) != "BCEND") {
		if (line_analyzer::isEmpty(line)) continue;
		std::string section = line;
		line = this->FR->readln();
		std::vector<std::string> list = line_analyzer::split(line, ',');
		for (std::string s : list) {
			this->BC->addByte(section, (char)stoi(s));
		}
	}

	// code loaded
	// section list now
	while ((line = this->FR->readln()) != "SLEND") {
		if (line_analyzer::isEmpty(line)) continue;
		std::string section = line;
		std::vector<std::string> list = line_analyzer::split(line, ',');
		if (list[0] != "UNDEFINED" && list[0] != "ABSOLUTE")this->SL->add(list[0], stoi(list[1]));
	}
}

void File::delete_file()
{
	this->FR->delete_file();
}
