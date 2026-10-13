#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* , uint32_t* other));
CSF_FUNC(int, __fastcall, C_00860378_ResolveOrDelegate, 0x006EB820, (C_00860378* self, void* , uint32_t* query, int* result));

static int __fastcall C_00860388_ResolveSelfOrDelegate(C_00860388* self, void* /*edx*/, uint32_t* a2, uint32_t* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BB46C))))
    {
        *a3 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(self));
        return self != nullptr;
    }

    C_00860378_ResolveOrDelegate(reinterpret_cast<C_00860378*>(self), nullptr, a2, reinterpret_cast<int*>(a3));
    return *a3 != 0;
}

HOOK(0x006B6A80, C_00860388_ResolveSelfOrDelegate, "auto");
