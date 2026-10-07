#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x00725550 (7 bytes) -- __thiscall, ret 0
// Returns the byte stored at this+0x1bc (a field declared only in a base class,
// so it is not present in C_00865328's field list).
static char __fastcall C_00865328_GetFlag01bc(C_00865328* self, void* /*edx*/)
{
    return static_cast<char>(CSF_AT(uint8_t, self, 0x1bc));
}

HOOK(0x00725550, C_00865328_GetFlag01bc, "auto");
