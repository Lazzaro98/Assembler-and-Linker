#include "../inc/Linker.h"
#include "../inc/regex_rules.h"
void Linker::load_sections() // this function will load all all sections in a new symbol table. It will update only their sizes.
{
	std::map<std::string, File>::iterator it;

	for (it = this->loaded_files.begin(); it != this->loaded_files.end(); it++)
	{
		File f = it->second;
		std::list<symbol> sections = f.ST->get_sections();
		for (auto s : sections)
			if (!this->ST->exists(s.name)) { // ako do sada nismo dodali tu sekciju, dodamo je
				// Da bi ID-jevi isli redom
				symbol s_add = s;
				s_add.id = ids++;
				this->ST->add(s_add);
				this->section_start_addresses[s.name] = -1;
			}
			else {// ako jesmo, onda povecamo samo size :)
				f.ST->find(s.name).pomeraj = this->ST->find(s.name).size;
				this->ST->find(s.name).size += s.size;
			}
	}
}

void Linker::load_globals()
{
	std::map<std::string, File>::iterator it;

	for (it = this->loaded_files.begin(); it != this->loaded_files.end(); it++)
	{
		File f = it->second;
		std::list<symbol> globals = f.ST->get_globals();
		for (auto s : globals)
			if (!this->ST->exists(s.name)) {
				symbol s_add = s;
				s_add.id = ids++;
				s_add.value += f.ST->find(s_add.section).pomeraj + this->section_start_addresses[s_add.section];
				this->ST->add(s_add);
			}
			else if (s.name != "ABSOLUTE")
				throw "Multiple definition of symbol " + s.name;
	}
}

void Linker::check_externs()
{
	std::map<std::string, File>::iterator it;
	std::set<symbol> externs;
	for (it = this->loaded_files.begin(); it != this->loaded_files.end(); it++) {
		File f = it->second;
		std::list<symbol> local_externs = f.ST->get_externs();
		for (auto s : local_externs) {
			if (s.name != "UNDEFINED")
				externs.insert(s);
		}
	} // kada smo skupili sve externe u jedan set, sada cemo da vidimo da li su oni svi definisani svaki bar u jednom fajlu.
	int cnt = 0;
	for (it = this->loaded_files.begin(); it != this->loaded_files.end(); it++)
	{
		File f = it->second;
		std::list<symbol> local_externs = f.ST->get_externs();
		for (auto s : local_externs)
			if (this->ST->exists(s.name)) { // ako do sada nismo dodali tu sekciju, dodamo je
				cnt++;
			}
	}
	if (externs.size() > cnt) { // brojimo cnt-om koliko smo externa razresili. Ako smo razresili onoliko externa koliko je postojalo, gg.
		std::string undefined_extern_symbols = "";
		for (auto s : externs)
			undefined_extern_symbols += s.name + ", ";
		throw "Symbols " + undefined_extern_symbols + " are declared as extern, but have never been defined anywhere.";
	}
}

void Linker::load_reloc()
{

	std::map<std::string, File>::iterator it;

	for (it = this->loaded_files.begin(); it != this->loaded_files.end(); it++)
	{
		File f = it->second;
		for (auto& entry : f.RT->get_map()) {
			std::string section = entry.first;
			std::list<rtentry> reloc_entry;

			for (rtentry r : entry.second) {
				symbol s_old = f.ST->find(r.value); // ovo ce biti ili sekcija, ili globalni simbol

				r.value = this->ST->find(s_old.name).id;
				r.offset += this->ST->find(section).pomeraj;
				//r.offset += this->section_start_addresses[section];
				//std::cout << r.offset<<std::endl;

				if (r.type == "R_386_16") { //TODO:
					int l = r.offset - this->ST->find(section).pomeraj;

					std::vector<char> v = this->BC->get_code_by_section(section);

					char c_high = v[l];
					char c_low = v[l + 1];
					int nadjeno = c_high;
					nadjeno <<= 8;
					nadjeno = nadjeno | c_low;
					int k = this->ST->find(s_old.name).value - this->ST->find(section).pomeraj + nadjeno;

					this->BC->update(section, l, k);
				}
				else {
					int l = r.offset - this->ST->find(section).pomeraj;

					std::vector<char> v = this->BC->get_code_by_section(section);

					char c_high = v[l];
					char c_low = v[l + 1];
					int nadjeno = c_high;
					nadjeno <<= 8;
					nadjeno = nadjeno | c_low;

					int k = 0;
					if (!f.ST->is_section(r.value))
						k = this->ST->find(s_old.name).value - r.offset + nadjeno;
					else
						l = this->ST->find(r.value).pomeraj - r.offset + nadjeno;
					this->BC->update(section, l, k);
				}
				
				if (!(this->ST->find(r.value).name == section))
					reloc_entry.push_back(r);
			}

			this->RT->append(section, reloc_entry);
		}
	}

}



void Linker::resolve_start_addresses()
{
	int layer = 0;
	for (auto s : this->input_start_addresses) {
		this->section_start_addresses[s.first] = s.second;
		if (is_overlap(s.first))
			throw "Memorija sekcija " + s.first + " se preklapa.";

		if (s.second + this->ST->find(s.first).size > layer)
			layer = s.second + this->ST->find(s.first).size;
	}
	// sada kada smo postavili sve user definisane lokacije, treba da popunimo lokacije onih sekcija koje user nije definisao na ulazu

	for (auto& s : this->section_start_addresses) {
		if (s.second == -1) {
			s.second = layer;
			layer += this->ST->find(s.first).size;
		}
	}
	for (auto p : section_start_addresses) {
		this->ST->find(p.first).value = p.second;
	}

	this->BC->load_start_addresses(section_start_addresses);
}

bool Linker::is_overlap(std::string sectionName) // proverava da li se sekcija overlapuje sa nekim trenutno definisanim lokacijama sekcijama
{
	int start_my = this->section_start_addresses[sectionName];
	int end_my = start_my + this->ST->find(sectionName).size;
	for (auto section : this->section_start_addresses) {
		if (section.second != -1) {
			int start_iter = section.second;
			int end_iter = section.second + this->ST->find(section.first).size;

			if ((start_my > start_iter && start_my < end_iter)
				|| (end_my > start_iter && end_my < end_iter))
				return true;
		}
	}
	return false;
}


void Linker::merge_code()
{
	std::map<std::string, File>::iterator it;

	for (it = this->loaded_files.begin(); it != this->loaded_files.end(); it++)
	{
		File f = it->second;
		std::list<symbol> sections = f.ST->get_sections();
		for (auto s : sections)
			this->BC->addByteArray(s.name, f.BC->get_code_by_section(s.name));
	}
}



void Linker::load_arguments(int argc, char* argv[])
{
	this->mode = "";
	int i = 1;
	for (i = 1; i <= argc; i++) {// prodjemo kroz sve argumente
		std::string argument = argv[i];
		if (argument == "-hex") { // prvi argument
			this->mode = "hex";
			continue;
		}
		if (argument == "-linkable") {
			this->mode = "linkable";
			continue;
		}

		if (argument.substr(0, 6) == "-place") {
			std::regex regex_linker_arguments(regex_rules::regex_linker_arguments);
			std::smatch match;
			std::regex_match(argument, match, regex_linker_arguments);
			std::string section = match.str(1);
			int start_address = std::stoi(match.str(2), 0, 16);
			this->input_start_addresses[section] = start_address;
		}

		if (argument == "-o") {
			i++;
			argument = argv[i];
			this->output_filename = argument;
			argument = argv[++i];
			// ucitali smo outout file name
		} // ostaje nam da ucitamo samo jos output_filename koji je sledeci, a onda i sve ulazne fajlove

		if (argument.substr(0, 1) != "-") {
			argument = argv[i];
			while (i < argc) {
				std::string s = argv[i++];
				s = s + "l";
				this->input_files.push_back(s);
			}
		}
	}
	//ucitali smo i sve input fajlove, jej
	if (this->mode == "")throw "Niste specificirali mod linkera.";
}

void Linker::receive_from_assembler()
{
	for (std::string file_name : input_files)
	{
		File* f = new File(file_name); // ucitaj File 
		f->receive_from_assembler(); // procitaj assembler-ov log za taj fajl
		this->loaded_files[file_name] = *f; // dodaj taj objekat u mapu svih fajlova
	}
}

void Linker::delete_ol_files() {
	for (std::string file_name : input_files)
	{
		File* f = new File(file_name); // ucitaj File 
		f->FR->delete_file();
	}
}

void Linker::output()
{
	if (this->mode == "hex")
		this->save_code_hex_in_file();
	if (this->mode == "linkable") {
		this->save_symbol_table_in_file();
		this->save_relocation_tables_in_file();
		this->save_code_in_file();
	}
	//delete_ol_files();
}




void Linker::link()
{

	this->receive_from_assembler();
	this->load_sections();
	this->check_externs();
	if (this->mode == "hex")
		this->resolve_start_addresses();
	else
		for (auto& s : this->section_start_addresses)
			s.second = 0;
	this->load_globals();
	this->merge_code();
	this->load_reloc();

}

void Linker::print_symbol_table()
{
	this->ST->print();
}

void Linker::print_relocation_table()
{
	this->RT->print();
}

void Linker::print_code()
{
	this->BC->print();
}

void Linker::print_hex()
{
	this->BC->print_hex();
}

void Linker::save_symbol_table_in_file()
{
	this->ST->print_save(this->output_filename);
}

void Linker::save_code_in_file()
{
	this->BC->print_save(this->output_filename);
}

void Linker::save_relocation_tables_in_file()
{
	this->RT->print_save(this->output_filename);
}

void Linker::save_code_hex_in_file()
{
	this->BC->print_hex_save(this->output_filename);
}

