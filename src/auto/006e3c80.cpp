#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __fastcall, Skin_DelegateResolve, 0x006B2840, (int a1, int a2, uint32_t* a3, int* a4));

static int __fastcall C_00860de0_ResolveThroughBase(C_00860de0* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBA9C))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    Skin_DelegateResolve((int)self, 0, a2, a3);
    return *a3 != 0;
}

HOOK(0x006E3C80, C_00860de0_ResolveThroughBase, "auto");
