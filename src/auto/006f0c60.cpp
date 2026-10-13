#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00860a30_DelegateResolve, 0x006C9B90, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_00862390_ResolveFlagOrDelegate(C_00862390* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB08))))
    {
        *a3 = (uint32_t)(uintptr_t)self;
        return self != nullptr;
    }

    C_00860a30_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006F0C60, C_00862390_ResolveFlagOrDelegate, "auto");
