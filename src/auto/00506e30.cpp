#include <cstdint>
#include "hook.h"
#include "core/memory.h"

// Returns the byte flag stored at offset 0x1708 of the object.
// Original: mov al, byte ptr [ecx + 0x1708]  (ret 0 -> __thiscall, this in ecx)
static char __fastcall C_00506e30_GetFlag1708(uint8_t* self, void* /*edx*/)
{
    return CSF_AT(char, self, 0x1708);
}

HOOK(0x506E30, C_00506e30_GetFlag1708, "auto");
