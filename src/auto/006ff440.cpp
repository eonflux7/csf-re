#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00863070_ResolveOrDelegate, 0x0070A380, (C_00863070* self, void* /*edx*/, uint32_t* a2, int* a3));

static int __fastcall C_008630e0_DelegateResolve(C_008630e0* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBE48))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00863070_ResolveOrDelegate(reinterpret_cast<C_00863070*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006FF440, C_008630e0_DelegateResolve, "auto");
