
#include "../inc/Assembler.h"
#include "../inc/Linker.h"
#include "../inc/File.h"
void load_program_arguments(int argc, char** argv, std::string* input_file_name, std::string* output_file_name) {
	if (argc > 1) {
		if (argv[2] != "-o" && argv[2] != "-O") {
			*input_file_name = argv[3];
			*output_file_name = argv[2];
		}
		else {
			*input_file_name = argv[2];
			*output_file_name = argv[3];
		}
	}
}
/*
int main(int argc, char** argv)
{
	try {

		std::string output_file_name = "output.o"; // default values
		std::string input_file_name = "input.s";
		//load_program_arguments(argc, argv, &input_file_name, &output_file_name); // load if there are program arguments
		Assembler* assembler = new Assembler(input_file_name, output_file_name);
		assembler->first_pass();
		assembler->second_pass();
		assembler->save_symbol_table_in_file(output_file_name);
		assembler->save_relocation_tables_in_file(output_file_name);
		assembler->save_code_in_file(output_file_name);
		assembler->send_all_to_linker();

	}
	catch (std::string msg) {
		std::cout << "ERROR: ";
		std::cout << msg << '\n';
	}
}


*/