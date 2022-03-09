#include "../Util/FileReader.h"
#include <iostream>
FileReader::FileReader(std::string filename)
{
	this->input_filename = filename;
}
void FileReader::set_input_filename(std::string filename)
{
	this->input_filename = filename;
}
std::string FileReader::get_input_filename()
{
	return this->input_filename;
}
bool FileReader::load(std::string filename)
{
	this->input_filename = filename;
	file.close();
	file.open(this->input_filename, std::fstream::in);

	if (file.is_open()) { return true; }
	throw "File " + this->input_filename + " doesn't exist.";
	return false; // this will never execute anyway
}
bool FileReader::load()
{
	return load(this->input_filename);
}
bool FileReader::reload()
{
	return load(this->input_filename);
}

std::string FileReader::readln()
{
	if (!this->file.is_open()) {
		throw "File " + this->input_filename + " was not loaded. Load it first.";
		return "err"; // this will never be executed anyway
	}

	std::string line;
	if (std::getline(this->file, line)) return line;
	else return "EOF";
}

void FileReader::delete_file()
{
	this->file.close();
	int status = std::remove(this->input_filename.c_str());
}
