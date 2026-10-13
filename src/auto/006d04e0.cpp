#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* edx, uint32_t* other));
CSF_FUNC(int, __fastcall, C_00860db0_ResolveFlagOrDelegate, 0x006F7FD0, (C_00860db0* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00860db0_TryResolveTarget(C_00860db0* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4F0))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860db0_ResolveFlagOrDelegate(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006D04E0, C_00860db0_TryResolveTarget, "auto");
