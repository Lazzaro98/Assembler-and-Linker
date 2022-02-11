
#include "Assembler.h"
#include "Linker.h"
#include "File.h"
void load_program_arguments(int argc, char** argv, std::string* input_file_name, std::string* output_file_name) {
	if (argc > 1) {
		if (argv[1] != "-o" && argv[1] != "-O") {
			*input_file_name = argv[1];
			*output_file_name = argv[3];
		}
		else {
			*input_file_name = argv[3];
			*output_file_name = argv[2];
		}
	}
}
/*
int main(int argc, char** argv)
{
    try {

		std::string output_file_name = "output.o"; // default values
		std::string input_file_name = "test.s";
		Linker* linker = new Linker(argc, argv);
		linker->load_arguments(argc, argv);
		linker->receive_from_assembler();
		linker->link();
		linker->output();
		linker->print_relocation_table();
		linker->print_symbol_table();
    }
    catch (std::string msg) {
        std::cout << "ERROR: ";
        std::cout << msg << '\n';
    }
}

*/


int main(int argc, char** argv)
{
	try {

		std::string output_file_name = "output.o"; // default values
		std::string input_file_name = "test2.txt";
		//load_program_arguments(argc, argv, &input_file_name, &output_file_name); // load if there are program arguments
		Assembler* assembler = new Assembler(input_file_name, output_file_name);
		assembler->first_pass();
		assembler->second_pass();
		assembler->save_symbol_table_in_file();
		assembler->save_relocation_tables_in_file();
		assembler->save_code_in_file();
		assembler->send_all_to_linker();

	}
	catch (std::string msg) {
		std::cout << "ERROR: ";
		std::cout << msg << '\n';
	}
}


