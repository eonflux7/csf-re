#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* edx, uint32_t* other));
CSF_FUNC(int, __fastcall, C_00860924_DelegateResolve, 0x006EFC30, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00860954_ResolveOrDelegate(C_00860954* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4D0))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860924_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006C8C70, C_00860954_ResolveOrDelegate, "auto");
