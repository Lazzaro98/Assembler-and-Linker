# Assembler-and-Linker
An implementation of 

 - two pass assembler that converts an assembly program into machine
   code and
 - linker which links the output of assembler into one file depending on
   which mode is chosen (-hex or -linkable)

## Project details

 - the project is compiled using g++ and tested on ubuntu
 - the assembly language used is x86-like, and supports sections,
   directives and some instructions with various addressing modes
 - the output of assembler is a file structured like an ELF object file,
   containing a symbol table, relocation data and machine code
 - the output of linker is either
	 -  **-linkable** which means a file structured like an ELF object file, containing a symbol table, relocation data and machine code or
	 - **-hex** the machine code with respected and aligned starting memory addresses for each section

## Project structure
#### Assembler
 - Contains header and source files of Assembler and main class.
 #### Linker
 - Contains header and source files of Linker and main class.
#### Util
 - Contains header and source files of all the classes that were beside above mentioned ones. They contain useful methods, structures, regex rules, etc.
#### Tests
 - Contains examples of tests.
## Usage

    ./assembler -o output.txt input.s
    ./linker -hex -place=section_name@0x0000 -o output input1 input2

