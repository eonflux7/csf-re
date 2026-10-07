#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x006D59F0: returns the single-precision float stored at offset 0x9c.
// Disassembly: fld dword ptr [ecx+0x9c] ; ret  -> callee pops 0 bytes (__thiscall, no args).
static double __fastcall C_00864508_GetField9c(C_00864508* self, void* /*edx*/)
{
    return self->field_009c;
}

HOOK(0x006D59F0, C_00864508_GetField9c, "auto");
