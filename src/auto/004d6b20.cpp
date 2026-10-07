#include <cstdint>
#include "globals.h"
#include "hook.h"

// __thiscall: stores the given 32-bit value at this+0x58 and returns it unchanged.
static int __fastcall C_Unknown_StoreValueAt0058(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x58) = value;
    return value;
}

HOOK(0x4D6B20, C_Unknown_StoreValueAt0058, "auto");
