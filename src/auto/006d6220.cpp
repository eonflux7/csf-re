#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_008610c0_ResolveOrDelegate, 0x006DB6C0, (C_008610c0* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_00861120_ResolveOrDelegate(C_00861120* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB708))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_008610c0_ResolveOrDelegate(reinterpret_cast<C_008610c0*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006D6220, C_00861120_ResolveOrDelegate, "auto");
