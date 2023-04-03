#ifndef COMMON_H
#define COMMON_H

typedef enum SymbolType
{
    SYM_TYPE_UNKNOWN=0,
    SYM_TYPE_FUNC=1,
    SYM_TYPE_DATA=2
} SymType;

typedef enum SymbolScope
{
    SYM_SCOPE_UNKNOWN=0,
    SYM_SCOPE_LOCAL=1,
    SYM_SCOPE_GLOBAL=2,
    SYM_SCOPE_WEAK=3,
} SymScope;

typedef enum Binary_Type
{
    Binary_Type_AUTO=0,
    Binary_Type_ELF=1,
    Binary_Type_PE=2,
}BinType;

/// Architecture type
typedef enum BinaryArch
{
    ARCH_NONE=-1,
    ARCH_ARM = 0,	///< ARM architecture (including Thumb, Thumb-2)
    ARCH_ARM64,		///< ARM-64, also called AArch64
    ARCH_X86		///< X86 architecture (including x86 & x86-64)
}BinArch;

/// Mode type
typedef enum Mode {
    MODE_LITTLE_ENDIAN = 0,	///< little-endian mode (default mode)
    MODE_ARM = 0,	///< 32-bit ARM
    MODE_16 = 1 << 1,	///< 16-bit mode (X86)
    MODE_32 = 1 << 2,	///< 32-bit mode (X86)
    MODE_64 = 1 << 3,	///< 64-bit mode (X86, PPC)
} BinMode;

typedef enum Section_Type
{
    SEC_TYPE_NONE=0,
    SEC_TYPE_CODE=1,
    SEC_TYPE_DATA=2
} SecType;

enum DISASM_TYPE
{
    DISASM_LINEAR=0,
    DISASM_RECURSIVE=1
};

#endif // COMMON_H


