#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __fastcall, Skin_DelegateResolve, 0x006B2840, (int a1, int a2, uint32_t* a3, int* a4));

static int __fastcall C_00861a60_ResolveOrDelegate(C_00861a60* self, void* /*edx*/, C_00860178* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB458))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    Skin_DelegateResolve((int)self, 0, reinterpret_cast<uint32_t*>(a2), a3);
    return *a3 != 0;
}

HOOK(0x006B1FB0, C_00861a60_ResolveOrDelegate, "auto");
