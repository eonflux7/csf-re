#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00863360_ResolveOrDelegate, 0x0070CB60, (C_00863360* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_008633a0_ResolveOrDelegate(C_008633a0* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBEE8))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00863360_ResolveOrDelegate(reinterpret_cast<C_00863360*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x00707500, C_008633a0_ResolveOrDelegate, "auto");
