#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* edx, uint32_t* other));
CSF_FUNC(int, __fastcall, C_008607b0_DelegateResolve, 0x006EEA00, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_008607f8_ResolveOrDelegate(C_008607f8* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4A0))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_008607b0_DelegateResolve(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006C0FF0, C_008607f8_ResolveOrDelegate, "auto");
