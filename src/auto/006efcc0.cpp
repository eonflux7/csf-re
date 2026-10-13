#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00860a30_ResolveOrDelegate, 0x006C9B90, (C_00860a30* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_00862240_ResolveOrDelegate(C_00862240* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB00))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860a30_ResolveOrDelegate(self, nullptr, reinterpret_cast<uint32_t*>(a2), a3);
    return *a3 != 0;
}

HOOK(0x006EFCC0, C_00862240_ResolveOrDelegate, "auto");
