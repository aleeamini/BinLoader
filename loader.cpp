#include "loader.h"
#include "bfd_interface.h"

int load_binary(std::string binfile,Binary* binary)
{
    bfd* bfd_obj;
    bfd_obj=BFD::binary_load(binfile);
    if(!bfd_obj)
        return -1;

    binary->set_name(binfile);
    binary->set_type(BFD::binary_get_type(bfd_obj));
    binary->set_type_str(BFD::binary_get_type_string(bfd_obj));

    binary->set_arch_str(BFD::binary_get_arch_str(bfd_obj));
    binary->set_arch_bits(BFD::binary_get_arch_bits(bfd_obj));
    binary->set_arch(BFD::binary_get_arch(bfd_obj));

    binary->set_entrypoint(BFD::binary_get_vma(bfd_obj));
    if(BFD::binary_load_symbols(bfd_obj,binary->symbols)!=0)
        return -1;
    if(BFD::binary_load_dyn_symbols(bfd_obj,binary->symbols)!=0)
        return -1;
    if(BFD::binary_load_sections(bfd_obj,binary->sections)!=0)
        return -1;
    return 0;
}
void show_symbols(Binary* binary,uint8_t just_dyn_symbols)
{
    std::string tmp="";
    printf("%-4s\t%-16s\t%-6s\t%-6s\t%s\n","Num","VMA","Type","Scope","Name");
    for(size_t i=0;i<binary->symbols.size();i++)
    {
        if(just_dyn_symbols==1 && binary->symbols[i].get_is_dyn_sym()!=1)
            continue;
        switch(binary->symbols[i].get_scope())
        {
        case SYM_SCOPE_LOCAL:
            tmp="LOCAL";
            break;
        case SYM_SCOPE_GLOBAL:
            tmp="GLOBAL";
            break;
        case SYM_SCOPE_WEAK:
            tmp="WEAK";
            break;
        default:
            tmp="UNK";
            break;
        }

        printf("%04zu\t%016lx\t%-6s\t%-6s\t%s\n",i,
               binary->symbols[i].get_addr(),
               binary->symbols[i].get_type()==SymType::SYM_TYPE_FUNC?"FUNC":"DATA",
               tmp.c_str(),
               binary->symbols[i].get_name().c_str());
    }
}
void show_sections(Binary* binary)
{
    printf("%-4s\t%-16s\t%-6s\t%-4s\t%s\n","Num","VMA","Size","Type","Name");
    for(size_t i=0;i<binary->sections.size();i++)
    {
        printf("%04zu\t%016lx\t%06lx\t%4s\t%s\n",i,
               binary->sections[i].get_vma(),
               binary->sections[i].get_size(),
               binary->sections[i].get_type()==SecType::SEC_TYPE_CODE?"CODE":"DATA",
               binary->sections[i].get_name().c_str());
    }
}

void dump_section(Binary* binary,std::string sec_name)
{

    size_t tmp=-1;
    for(size_t i=0;i<binary->sections.size();i++)
    {
        if(binary->sections[i].get_name()==sec_name)
        {
            tmp=i;
            break;
        }
    }
    if(tmp == -1)
    {
        printf("Error: there is no section with name %s:\n",sec_name.c_str());
        return;

    }
    printf("Contents of section %s:\n",sec_name.c_str());
    uint8_t* data=binary->sections[tmp].get_data();
    size_t i=0,j=0,k=0;
    unsigned int vma=binary->sections[tmp].get_vma();
    while(i<binary->sections[tmp].get_size())
    {
        printf("%x  ",vma);
        k=i;
        for(j=0;j<8;j++)
        {
            printf("%02x%02x ",data[i],data[i+1]);
            i+=2;
        }
        for(j=0;j<16;j++)
        {
            if(isprint(data[k]))
                printf("%c",data[k]);
            else
                printf(".");

            k++;
        }
        vma+=16;
        printf("\n");
    }
}
void remove_weak_symbols(Binary* binary)
{
    std::vector<Symbol>::iterator symbol = binary->symbols.begin();
    std::vector<Symbol>::iterator symbol2 = binary->symbols.begin();
    for(; symbol< binary->symbols.end();symbol++)
    {
        if(symbol->get_scope()==SYM_SCOPE_WEAK)
        {
            for(; symbol2< binary->symbols.end();symbol2++)
            {
                if(symbol2->get_name() == symbol->get_name() && symbol2->get_scope()!=SYM_SCOPE_WEAK)
                {
                    binary->symbols.erase(symbol);
                    break;
                }
            }
        }
    }
}
