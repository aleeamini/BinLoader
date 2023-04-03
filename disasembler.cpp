#include "disasembler.h"
#include <map>
/*
 * init disasm handler with default  arch and mode: x86 32 bit
*/
Disasmembler::Disasmembler()
{
    this->disasm_handler=0;
    if(cs_open(cs_arch::CS_ARCH_X86,cs_mode::CS_MODE_64,&(this->disasm_handler))!=CS_ERR_OK)
    {
        fprintf(stderr,"faild to open handle to capstone.\n");
        return;
    }
}
/*
 * init disasm handler with arch and mode
*/
Disasmembler::Disasmembler(BinArch arch,BinMode mode)
{
    this->disasm_handler=0;
    if(cs_open((cs_arch)arch,(cs_mode)mode,&disasm_handler)!=CS_ERR_OK)
    {
        fprintf(stderr,"faild to open handle to capstone\n");
        return;
    }
}
/*
 * get a buffer of opcodes and disasmble base on type
 *
*/
int Disasmembler::disasm(uint8_t* buffer,size_t size,uint64_t start_addr,DISASM_TYPE disasm_type)
{
    if(disasm_type==DISASM_LINEAR)
        return disasm_linear(buffer, size, start_addr);
    else if(disasm_type==DISASM_RECURSIVE)
        return disasm_iter(buffer, size, start_addr);
    return -1;
}
int Disasmembler::disasm_linear(uint8_t* buffer,size_t size,uint64_t start_addr)
{
    if(disasm_handler==0)
        return -1;
    this->instructions_number=cs_disasm(this->disasm_handler,buffer,size,start_addr,0,&this->instructions);
    if(this->instructions_number < 0)
    {
        fprintf(stderr,"Disassembly error: %s\n",cs_strerror(cs_errno(this->disasm_handler)));
        return -1;
    }
    return this->instructions_number;
}
bool Disasmembler::is_cflow_group(uint8_t g)
{
    if(g==CS_GRP_CALL || g==CS_GRP_JUMP || g==CS_GRP_IRET || g==CS_GRP_RET)
        return true;
    return false;
}
bool Disasmembler::is_cflow_ins(cs_insn* ins)
{
    for(size_t i=0;i<ins->detail->groups_count;i++)
     {
        if( ins->detail->groups[i]==CS_GRP_CALL || ins->detail->groups[i]==CS_GRP_JUMP ||
            ins->detail->groups[i]==CS_GRP_IRET || ins->detail->groups[i]==CS_GRP_RET)
            return true;
    }
    return false;

}
void Disasmembler::print_ins(cs_insn* ins)
{
    printf("0x%016jx: ",ins->address);
    for(size_t j=0; j< 16; j++)
    {
        if(j < ins->size)
            printf("%02x " , ins->bytes[j]);
        else
            printf("   ");
    }
    printf("%-12s %s\n", ins->mnemonic,ins->op_str);
}

int Disasmembler::disasm_iter(const uint8_t* buffer,size_t size, uint64_t start_addr)
{
    std::map<uint64_t,bool> seen;
    this->instructions=cs_malloc(this->disasm_handler);
    while(cs_disasm_iter(disasm_handler,&buffer,&size,&start_addr,this->instructions))
    {
        if(this->instructions->id==X86_INS_INVALID || this->instructions->size==0)
            break;
        seen[this->instructions->address]=true;
        this->print_ins(this->instructions);


    }
}




Disasmembler::~Disasmembler()
{
    cs_free(this->instructions,instructions_number);
    cs_close(&this->disasm_handler);
    this->disasm_handler=0;
}
