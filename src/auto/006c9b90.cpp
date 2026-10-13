#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00860990_ResolveFallback, 0x006CA7C0, (void* self, void* edx, uint32_t* a2, int* a3));

static int __fastcall C_00860a30_ResolveOrDelegate(C_00860a30* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB4D4))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860990_ResolveFallback(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006C9B90, C_00860a30_ResolveOrDelegate, "auto");
