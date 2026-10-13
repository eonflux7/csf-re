#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_008628b8_ResolveToSelfOrDelegate, 0x007080E0, (C_008628b8* self, void* , uint32_t* a2, int* a3));

static int __fastcall C_00862900_ResolveToSelfOrDelegate(C_00862900* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBB24))))
    {
        *a3 = (uint32_t)(uintptr_t)self;
        return self != nullptr;
    }

    C_008628b8_ResolveToSelfOrDelegate(reinterpret_cast<C_008628b8*>(self), nullptr, a2, reinterpret_cast<int*>(a3));
    return *a3 != 0;
}

HOOK(0x006F70F0, C_00862900_ResolveToSelfOrDelegate, "auto");
