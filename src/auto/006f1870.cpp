#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, sub_6C9B90, 0x006C9B90, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00862490_ResolveOrDelegate(C_00862490* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB0C))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    sub_6C9B90(self, nullptr, a2, reinterpret_cast<uint32_t*>(a3));
    return *a3 != 0;
}

HOOK(0x006F1870, C_00862490_ResolveOrDelegate, "auto");
