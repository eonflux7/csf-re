#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00861928_ResolveOrDelegate, 0x007027A0, (C_00861928* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_00861988_ResolveOrDelegate(C_00861988* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB734))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00861928_ResolveOrDelegate(reinterpret_cast<C_00861928*>(self), nullptr, a2, a3);
    return *a3 != 0;
}

HOOK(0x006DF7C0, C_00861988_ResolveOrDelegate, "auto");
