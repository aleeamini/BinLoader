#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdint.h>
#include <string>
#include "common.h"

class Symbol
{
private:

    std::string name;
    uint64_t    addr;
    SymType     type;
    SymScope    scope;
    uint8_t     is_dyn_sym;

public:
    Symbol()
    {
        type=SYM_TYPE_UNKNOWN;
        scope=SYM_SCOPE_UNKNOWN;
        name="";
        addr=0;
        is_dyn_sym=0;
    }
    void set_name(std::string str)
    {
        this->name=str;
    }
    std::string get_name()
    {
        return this->name;
    }
    void set_type(SymbolType stype)
    {
        this->type=stype;
    }
    SymbolType get_type()
    {
        return this->type;
    }
    void set_scope(SymbolScope sscope)
    {
        this->scope=sscope;
    }
    SymbolScope get_scope()
    {
        return this->scope;
    }
    void set_addr(uint64_t address)
    {
        this->addr=address;
    }
    uint64_t get_addr()
    {
        return this->addr;
    }
    void set_is_dyn_sym()
    {
        this->is_dyn_sym=1;
    }
    uint8_t get_is_dyn_sym()
    {
        return this->is_dyn_sym;
    }

};

#endif // SYMBOL_H
