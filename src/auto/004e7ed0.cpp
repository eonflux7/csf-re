#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: sub_4E7E30 (__thiscall-style, one stack arg, ret 4)
CSF_FUNC(char, __fastcall, sub_4E7E30, 0x004E7E30, (void* self, void* edx, int a2));

// Processes the state slot at (a2 + 0x14) owned by this object. If the callee
// reports success, the block at a2 is moved to state 3 with a timer/param of 12.
// Returns non-zero unless the state at a2+4 is -1 (cancelled).
static int __fastcall C_0083eb70_ProcessStateSlotAndTestValid(C_0083eb70* self, void* /*edx*/, int a2)
{
    if (sub_4E7E30(self, nullptr, a2 + 20))
    {
        CSF_AT(int, reinterpret_cast<uint8_t*>(a2), 0x04) = 3;
        CSF_AT(int, reinterpret_cast<uint8_t*>(a2), 0x08) = 12;
    }
    return CSF_AT(int, reinterpret_cast<uint8_t*>(a2), 0x04) != -1 ? 1 : 0;
}

HOOK(0x004E7ED0, C_0083eb70_ProcessStateSlotAndTestValid, "auto");
