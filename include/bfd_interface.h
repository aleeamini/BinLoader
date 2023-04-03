#ifndef BFD_INTERFACE_H
#define BFD_INTERFACE_H
#include <bfd.h>
#include "binary.h"
#include "common.h"

#define BFD64

class BFD
{
    static unsigned int  bfd_inited;
    static void init_bfd()
    {
        if(bfd_inited==0)
        {
            bfd_init();
            bfd_inited=1;
        }
    }
  public:
    BFD()
    {
        BFD::init_bfd();
    }
    /*
     * this function gets a file address and return a bfd pointer if file be a valid executable file
     *
     */

    static bfd* binary_load(const std::string fname)
    {
        bfd* bfd_bin;

        /*
         * this function gets file addr and the filetype as arguments. we set the second arg to null,
         *
        */
        bfd_bin=bfd_openr(fname.c_str(),NULL);
        if(!bfd_bin)
        {
            fprintf(stderr,"faild to open binary '%s': %s",fname.c_str(),bfd_errmsg(bfd_get_error()));
            return NULL;
        }
        if(!bfd_check_format(bfd_bin,bfd_object))
        {
            fprintf(stderr,"file '%s' isn't a valid binary file: %s\n",fname.c_str(),bfd_errmsg(bfd_get_error()));
            return NULL;
        }
        bfd_set_error(bfd_error_no_error);
        if(bfd_get_flavour(bfd_bin)==bfd_target_unknown_flavour)
        {
            fprintf(stderr,"file '%s' isn't a known binary file: %s\n",fname.c_str(),bfd_errmsg(bfd_get_error()));
            return NULL;
        }
        return bfd_bin;
    }
    /*
     * return the type of binary
     */
    static std::string binary_get_type_string(bfd* bin)
    {
        return std::string(bin->xvec->name);
    }
    static uint64_t binary_get_vma(bfd* bin)
    {
        return bfd_get_start_address(bin);
    }
    static Binary_Type binary_get_type(bfd* bin)
    {
        switch(bin->xvec->flavour)
        {
        case bfd_target_elf_flavour:
            return BinType::Binary_Type_ELF;
            break;
        case bfd_target_coff_flavour:
            return BinType::Binary_Type_PE;
            break;
        case bfd_target_unknown_flavour:
        default:
            fprintf(stderr,"file '%s' isn't a known binary file\n");
            return BinType::Binary_Type_AUTO;
            break;
        }
    }

    static std::string binary_get_arch_str(bfd* bin)
    {
        const bfd_arch_info_type* bfd_info;
        bfd_info = bfd_get_arch_info(bin);
        return std::string(bfd_info->printable_name);

    }
    static BinaryArch binary_get_arch(bfd* bin)
    {
        const bfd_arch_info_type* bfd_info;
        bfd_info = bfd_get_arch_info(bin);
        switch(bfd_info->mach)
        {
        case bfd_mach_i386_i386:
            return BinArch::ARCH_X86;
            break;
        case bfd_mach_x86_64:
            return BinArch::ARCH_X86;
            break;
        default:
            return BinArch::ARCH_NONE;

        }
    }
    static unsigned binary_get_arch_bits(bfd* bin)
    {
        const bfd_arch_info_type* bfd_info;
        bfd_info = bfd_get_arch_info(bin);
        switch(bfd_info->mach)
        {
        case bfd_mach_i386_i386:
            return 32;
            break;
        case bfd_mach_x86_64:
            return 64;
            break;
        default:
            return 0;
        }
    }
    static int binary_load_symbols(bfd* bin,std::vector<Symbol>& syms)
    {
        long n=0,sym_count=0,ret=0;
        asymbol **bfd_symtable;
        Symbol *tmp;
        //get the size of symtabele in bytes
        n=bfd_get_symtab_upper_bound(bin);
        if(n < 0 )
        {
            fprintf(stderr,"faild to read symtable:(%s)\n",  bfd_errmsg(bfd_get_error()));
            ret=-1;
            goto cleanup;
        }
        bfd_symtable=(asymbol**)malloc(n);
        if(!bfd_symtable)
        {
            fprintf(stderr,"malloc faild.out of memory\n");
            ret=-1;
            goto cleanup;
        }
        sym_count = bfd_canonicalize_symtab(bin,bfd_symtable);
        if(sym_count<0)
        {
            fprintf(stderr,"faild to read symtable:(%s)\n", bfd_errmsg(bfd_get_error()));
            ret=-1;
            goto cleanup;
        }
        for(int i=0;i<sym_count;i++)
        {
            if(bfd_symtable[i]->flags & BSF_FUNCTION || bfd_symtable[i]->flags & BSF_OBJECT)
            {
                tmp=new Symbol();
                tmp->set_type(bfd_symtable[i]->flags & BSF_FUNCTION?SymType::SYM_TYPE_FUNC:SymType::SYM_TYPE_DATA);
                tmp->set_name(std::string(bfd_symtable[i]->name));
                tmp->set_addr(bfd_asymbol_value(bfd_symtable[i]));
                if(bfd_symtable[i]->flags & BSF_LOCAL)
                    tmp->set_scope(SymbolScope::SYM_SCOPE_LOCAL);

                if(bfd_symtable[i]->flags & BSF_GLOBAL)
                    tmp->set_scope(SymbolScope::SYM_SCOPE_GLOBAL);

                if(bfd_symtable[i]->flags & BSF_WEAK)
                    tmp->set_scope(SymbolScope::SYM_SCOPE_WEAK);

                syms.push_back(*tmp);
            }
        }
        cleanup:
            if(bfd_symtable)
                free(bfd_symtable);
        return ret;
    }
    static int binary_load_dyn_symbols(bfd* bin,std::vector<Symbol>& syms)
    {
        flagword f;
        long n=0,sym_count=0,ret=0;
        asymbol **bfd_dyn_symtable;
        Symbol *tmp;
        //get the size of symtabele in bytes
        n=bfd_get_dynamic_symtab_upper_bound(bin);
        if(n < 0 )
        {
            fprintf(stderr,"faild to read dyn symtable:(%s)\n",  bfd_errmsg(bfd_get_error()));
            ret=-1;
            goto cleanup;
        }
        bfd_dyn_symtable=(asymbol**)malloc(n);
        if(!bfd_dyn_symtable)
        {
            fprintf(stderr,"malloc faild.out of memory\n");
            ret=-1;
            goto cleanup;
        }
        sym_count = bfd_canonicalize_dynamic_symtab(bin,bfd_dyn_symtable);
        if(sym_count<0)
        {
            fprintf(stderr,"faild to read dyn symtable:(%s)\n", bfd_errmsg(bfd_get_error()));
            ret=-1;
            goto cleanup;
        }
        symbol_info* syminfo;
        for(int i=0;i<sym_count;i++)
        {
            if(bfd_dyn_symtable[i]->flags & BSF_FUNCTION || bfd_dyn_symtable[i]->flags & BSF_OBJECT)
            {
                tmp=new Symbol();
                tmp->set_type(bfd_dyn_symtable[i]->flags & BSF_FUNCTION?SymType::SYM_TYPE_FUNC:SymType::SYM_TYPE_DATA);
                tmp->set_name(std::string(bfd_dyn_symtable[i]->name));
                tmp->set_addr(bfd_asymbol_value(bfd_dyn_symtable[i]));

                if((bfd_dyn_symtable[i]->flags & BSF_LOCAL) != 0)
                    tmp->set_scope(SymbolScope::SYM_SCOPE_LOCAL);

                if((bfd_dyn_symtable[i]->flags & BSF_GLOBAL) !=0)
                    tmp->set_scope(SymbolScope::SYM_SCOPE_GLOBAL);

                if((bfd_dyn_symtable[i]->flags & BSF_WEAK) !=0)
                    tmp->set_scope(SymbolScope::SYM_SCOPE_WEAK);

                else
                    tmp->set_scope(SymbolScope::SYM_SCOPE_GLOBAL);

                tmp->set_is_dyn_sym();
                syms.push_back(*tmp);
            }
        }
        cleanup:
            if(bfd_dyn_symtable)
                free(bfd_dyn_symtable);
            return ret;
    }
    static int binary_load_sections(bfd* bin,std::vector<Section>& sections)
    {
        asection *cur_section;
        int bfd_flags;
        cur_section=bin->sections;
        uint64_t vma, size=0;
        SecType type;
        const char *secname;
        uint8_t* tmp_data;
        Section* tmp_sec;
        while (cur_section!=NULL)
        {
            if(cur_section->flags & SEC_CODE)
                type=SEC_TYPE_CODE;
            else if(cur_section->flags & SEC_DATA)
                type=SEC_TYPE_DATA;
            else
            {
                cur_section=cur_section->next;
                continue;
            }
            vma=bfd_section_vma(cur_section);
            size=bfd_section_size(cur_section);
            secname=bfd_section_name(cur_section);
            if(!secname)
                secname="unnamed";
            tmp_data=(uint8_t*)malloc(size);
            tmp_sec=new Section();
            if(!tmp_data)
            {
                fprintf(stderr,"faild to alloc memory for section data\n");
            }
            else
            {
                if(!bfd_get_section_contents(bin,cur_section,tmp_data,0,size))
                {
                    fprintf(stderr,"faild to read data of section %s\n",secname);
                }
                else
                {
                    tmp_sec->set_data(tmp_data);
                }
            }
            tmp_sec->set_name(std::string(secname));
            tmp_sec->set_size(size);
            tmp_sec->set_vma(vma);
            tmp_sec->set_type(type);
            sections.push_back(*tmp_sec);

            cur_section=cur_section->next;
        }
        return 0;
    }
    static void binary_unload(Binary* bin)
    {

    }
};
unsigned int BFD::bfd_inited=0;

#endif // BFD_INTERFACE_H
