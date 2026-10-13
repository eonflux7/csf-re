#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: __fastcall(self, edx, int a2, unsigned int a3), returns char, pops 8.
CSF_FUNC(char, __fastcall, MemDebug_DispatchToOwner, 0x00656260, (void* self, void* edx, int a2, unsigned int a3));

// __thiscall(this = a2, arg0, a3) with ret 8 written as __fastcall(this, edx, arg0, a3).
static char __fastcall MemDebug_MarkFlagThenDispatch(uint8_t* self, void* /*edx*/, C_008622e8* arg0, char a3)
{
    if (a3)
        *self = 1;
    return MemDebug_DispatchToOwner(arg0, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(self)), 1u);
}

HOOK(0x690C00, MemDebug_MarkFlagThenDispatch, "auto");
