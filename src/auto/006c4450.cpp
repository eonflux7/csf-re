#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x006C4450 (__thiscall, ret 0):
//   return *(BYTE*)(*(this + 1) + 0x20);
// i.e. take the pointer stored at this+0x04 and return the byte at offset 0x20 of that object.
static char __fastcall GetChildByte0x20(void* self, void* /*edx*/)
{
    void* child = CSF_AT(void*, self, 0x4);
    return CSF_AT(char, child, 0x20);
}

HOOK(0x6C4450, GetChildByte0x20, "auto");
