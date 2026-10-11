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
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, Hud_CheckFlagAgainstGlobal, 0x006B2800, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_00860578_DelegateResolve, 0x006EE400, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_008605d8_ResolveOrDelegate(C_008605d8* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (Hud_CheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB490))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860578_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006BBFA0, C_008605d8_ResolveOrDelegate, "auto");
