#ifndef _FileReader_h_
#define _FileReader_h_


#define DEFAULT_INPUT_FILENAME "input.txt"
#define DEFAULT_OUTPUT_FILENAME "output.txt"

#include <fstream>
#include <string>

class FileReader
{
	std::fstream file;
	std::string input_filename;

	std::string output_filename;
public:
	FileReader(std::string filename = DEFAULT_INPUT_FILENAME);
	void set_input_filename(std::string filename);
	std::string get_input_filename();
	bool load(std::string filename);
	bool load();
	bool reload();
	std::string readln();
	void delete_file();
};

#endif
