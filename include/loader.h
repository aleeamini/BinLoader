#ifndef LOADER_H
#define LOADER_H

#include "binary.h"

int load_binary(std::string binfile,Binary* binary);
void show_symbols(Binary* binary,uint8_t just_dyn_symbols);
void show_sections(Binary* binary);
void dump_section(Binary* binary,std::string sec_name);
void remove_weak_symbols(Binary* binary);
#endif // LOADER_H
