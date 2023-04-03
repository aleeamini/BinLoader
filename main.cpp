#include <iostream>
#include <getopt.h>
#include <capstone/capstone.h>
#include "loader.h"
#include "common.h"
#include "disasm.h"

using namespace std;
struct arguments
{
    uint8_t help_flag;
    uint8_t verbose_flag;
    uint8_t show_symbols_flag;
    uint8_t show_dynsymbols_flag;
    uint8_t show_sections_flag;
    uint8_t dump_section_flag;
    uint8_t omit_weaks;
    std::string dump_section_name;
    std::string filename;
    uint8_t disasm;
    std::string disasm_section=".text";
    DISASM_TYPE disasm_type=(DISASM_TYPE)-1;

} args;



int main(int argc, char* argv[])
{
    int c;
    int option_index = 0;
    std::string tmp;
    if(argc<2)
    {
        std::cout<<"usage: "<<argv[0]<<" <binary-path>"<<"\n";
        return 1;
    }

    static struct option options[] =
    {
            //name                  options mode                flag    return_value
            {"dump",                required_argument,          NULL,       'd'},
            {"symbols",             no_argument,                NULL,       's'},
            {"dyn-symbols",         no_argument,                NULL,       'y'},
            {"sections",            no_argument,                NULL,       'e'},
            {"omit-weaks",          no_argument,                NULL,       'w'},
            {"disassemble",         optional_argument,          NULL,       'D'},
            {"disassemble-type",    required_argument,          NULL,       'T'},
            {"help",                no_argument,                NULL,       'h'},
            {"verbose",             no_argument,                NULL,       'v'},
            {NULL,                  0,                          NULL,       0}
    };
    while (1)
    {
        c = getopt_long(argc, argv, "-:d:syewD::T:hv", options, &option_index);
        if (c == -1)
            break;

        switch (c)
        {
        case 0:
            printf("long option %s", options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

        case 1:
            args.filename=std::string(optarg);
            break;

        case 'd':
            args.dump_section_flag=1;
            args.dump_section_name=std::string(optarg);
            break;

        case 'v':
            args.verbose_flag=1;
            break;

        case 'w':
            args.omit_weaks=1;
            break;

        case 'D':
            args.disasm=1;
            if(optarg)
                args.disasm_section=std::string(optarg);
            break;

        case 'T':
            tmp=std::string(optarg);
            if(tmp=="recursive")
                args.disasm_type=DISASM_TYPE::DISASM_LINEAR;
            else if(tmp=="linear")
                args.disasm_type=DISASM_TYPE::DISASM_RECURSIVE;

            break;

        case 's':
            args.show_symbols_flag=1;
            break;

        case 'y':
            args.show_dynsymbols_flag=1;
            break;

        case 'e':
            args.show_sections_flag=1;
            break;
        case 'h':
            args.help_flag=1;
            break;
        case '?':
            printf("Unknown option %c\n", optopt);
            break;
        case ':':
            printf("Missing option for %c\n", optopt);
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }
    if(args.filename.empty())
    {
        std::cout<<"usage: "<<argv[0]<<" <binary-path>"<<"\n";
        return 1;
    }
    Binary* bin=new Binary();
    if(load_binary(args.filename, bin)!=0)
    {
        fprintf(stderr,"There is a problem in loading binary.\n");
        return 1;
    }
    if(args.show_dynsymbols_flag==1)
        show_symbols(bin,1);
    if(args.show_symbols_flag==1)
        show_symbols(bin,0);
    if(args.show_sections_flag==1)
        show_sections(bin);
    if(args.dump_section_flag==1)
        dump_section(bin,args.dump_section_name);
    if(args.omit_weaks==1)
        remove_weak_symbols(bin);
    if(args.disasm==1)
    {
        if(args.disasm_type==(DISASM_TYPE)-1)
            disasm_bin(bin,DISASM_TYPE::DISASM_LINEAR);
        else
            disasm_bin(bin,args.disasm_type);

    }
    return 0;
}
