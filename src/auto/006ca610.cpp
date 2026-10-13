#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_008305f8_ResolveOrDelegate_6C4540, 0x006C4540, (C_008305f8* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_008305f8_CheckFlagOrResolve_6CA610(C_008305f8* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4C0))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_008305f8_ResolveOrDelegate_6C4540(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006CA610, C_008305f8_CheckFlagOrResolve_6CA610, "auto");
