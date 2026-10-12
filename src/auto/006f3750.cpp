#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_6C9B90, 0x006C9B90, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00862748_ResolveOrDelegate(C_00862748* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if ((uint8_t)Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB18))))
    {
        *a3 = (uint32_t)(uintptr_t)self;
        return self != nullptr;
    }

    sub_6C9B90(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006F3750, C_00862748_ResolveOrDelegate, "auto");
