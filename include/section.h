#ifndef SECTION_H
#define SECTION_H

#include <string>
#include "common.h"

class Section
{
private:
    SecType type;
    std::string name;
    uint64_t vma;
    uint64_t size;
    uint8_t* data;
public:
    Section()
    {
        type=SEC_TYPE_NONE;name="";vma=0;size=0;data=NULL;
    }
    void set_name(std::string str)
    {
        this->name=str;
    }
    std::string get_name()
    {
        return this->name;
    }
    void set_type(Section_Type stype)
    {
        this->type=stype;
    }
    Section_Type get_type()
    {
        return this->type;
    }
    void set_vma(uint64_t address)
    {
        this->vma=address;
    }
    uint64_t get_vma()
    {
        return this->vma;
    }
    void set_size(uint64_t size)
    {
        this->size=size;
    }
    uint64_t get_size()
    {
        return this->size;
    }
    void set_data(uint8_t* data)
    {
        this->data=data;
    }
    uint8_t* get_data()
    {
        return this->data;
    }

    bool is_there(uint64_t addr)
    {
        return (addr>= this->vma) && (addr-this->vma < this->size);
    }
};

#endif // SECTION_H
