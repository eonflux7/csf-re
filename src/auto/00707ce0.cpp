#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, sub_6C0FF0, 0x006C0FF0, (void* self, void* edx, uint32_t* a2, uint32_t* a3));

static int __fastcall C_008633e0_ResolveOrDelegate(C_008633e0* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBEEC))))
    {
        *a3 = (uint32_t)self;
        return self != nullptr;
    }

    sub_6C0FF0(self, nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x00707CE0, C_008633e0_ResolveOrDelegate, "auto");
