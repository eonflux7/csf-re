#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00863018_ResolveTarget, 0x006DB670, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00863044_ResolveFlagOrDelegate(C_00863044* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB40))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00863018_ResolveTarget(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006FE340, C_00863044_ResolveFlagOrDelegate, "auto");
