#include "disasm.h"
#include "disasembler.h"

void disasm_bin(Binary* bin,DISASM_TYPE disasm_type)
{
    Disasmembler* da=new Disasmembler();
    Section* text;
    for(size_t i=0;i<bin->sections.size();i++)
    {
        if(bin->sections[i].get_name()==".text")
        {
            text=&bin->sections[i];
            break;
        }
    }

    int n= da->disasm(text->get_data(),text->get_size(),text->get_vma(),DISASM_LINEAR);
    for(size_t i=0;i< n; i++)
    {
        printf("0x%016jx: ",da->instructions[i].address);
        for(size_t j=0; j< 16; j++)
        {
            if(j < da->instructions[i].size)
                printf("%02x " , da->instructions[i].bytes[j]);
            else
                printf("   ");
        }
        printf("%-12s %s\n", da->instructions[i].mnemonic,da->instructions[i].op_str);
    }
    delete da;
}
