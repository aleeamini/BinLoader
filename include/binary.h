#ifndef BINARY_H
#define BINARY_H

#include <vector>
#include <string>
#include "symbol.h"
#include "section.h"
#include <bfd.h>
#include "common.h"

class Binary
{
private:

    BinType type;
    std::string type_str;
    BinArch arch;
    std::string arch_str;
    unsigned    arch_bits;
    std::string name;
    uint64_t    entrypoint;

public:
    Binary()
    {
        type=Binary_Type_AUTO;type_str="";arch=ARCH_NONE;arch_str="";name="unnamed";entrypoint=0;arch_bits=0;
    }
    std::vector<Symbol> symbols;
    std::vector<Section> sections;
    void set_name(std::string str)
    {
        this->name=str;
    }
    std::string get_name()
    {
        return this->name;
    }
    void set_type(Binary_Type stype)
    {
        this->type=stype;
    }
    Binary_Type get_type()
    {
        return this->type;
    }
    void set_entrypoint(uint64_t address)
    {
        this->entrypoint=address;
    }
    uint64_t get_entrypoint()
    {
        return this->entrypoint;
    }
    void set_type_str(std::string type)
    {
        this->type_str=type;
    }
    std::string get_type_str()
    {
        return this->type_str;
    }
    void set_arch_str(std::string arch)
    {
        this->arch_str=arch;
    }
    std::string get_arch_str()
    {
        return this->arch_str;
    }
    void set_arch_bits(unsigned bits)
    {
        this->arch_bits=bits;
    }
    unsigned get_arch_bits()
    {
        return this->arch_bits;
    }
    void set_arch(BinArch arch)
    {
        this->arch=arch;
    }
    BinArch get_arch()
    {
        return this->arch;
    }


};

#endif // BINARY_H
