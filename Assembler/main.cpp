
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

int main(int argc, char** argv)
{
    try {

		std::string output_file_name = "output.txt"; // default values
		std::string input_file_name = "input.txt";
		load_program_arguments(argc, argv, &input_file_name, &output_file_name); // load if there are program arguments

		Assembler* assembler = new Assembler(input_file_name, output_file_name);
		assembler->first_pass();
		assembler->print_symbol_table();
		assembler->second_pass();
		assembler->print_relocation_tables();
		assembler->print_code_with_addresses();
		assembler->send_all_to_linker();
		/*Linker* linker = new Linker(argc, argv);
		
		linker->link();
		linker->print_symbol_table();
		linker->print_hex();
		linker->print_code();
		linker->save_code_hex_in_file();*/
    }
    catch (std::string msg) {
        std::cout << "ERROR: ";
        std::cout << msg << '\n';
    }
}

