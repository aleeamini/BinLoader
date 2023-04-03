#ifndef DISASM_H
#define DISASM_H

#include <stdint.h>
#include <capstone/capstone.h>
#include "common.h"


class Disasmembler
{
private:
    csh disasm_handler;
    size_t instructions_number;
    int disasm_linear(uint8_t* buffer,size_t size,uint64_t start_addr);
    int disasm_iter(const uint8_t* buffer,size_t size, uint64_t start_addr);
    bool is_cflow_group(uint8_t g);
    bool is_cflow_ins(cs_insn* ins);
    void print_ins(cs_insn* ins);
public:
    cs_insn* instructions;
    Disasmembler(BinArch arch,BinMode mode);
    Disasmembler();
    ~Disasmembler();
    int disasm(uint8_t* buffer,size_t size,uint64_t start_addr,DISASM_TYPE disasm_type);

};




#endif // DISASM_H
