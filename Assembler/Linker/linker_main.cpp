
#include "Linker.h"
#include "../Util/File.h"

int main(int argc, char** argv)
{
	try {

		std::string output_file_name = "output.hex"; // default values
		std::string input_file_name = "input.o";
		Linker* linker = new Linker(argc, argv);
		linker->load_arguments(argc, argv);
		linker->receive_from_assembler();
		linker->link();
		linker->output();
	}
	catch (std::string msg) {
		std::cout << "ERROR: ";
		std::cout << msg << '\n';
	}
}


