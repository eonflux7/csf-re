#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* /*edx*/, uint32_t* other));
CSF_FUNC(bool, __fastcall, C_00860178_ResolveDelegate, 0x006B2840, (C_00860178* self, void* /*edx*/, uint32_t* identifier, uint32_t* delegateOut));

static int __fastcall C_00860924_ResolveDelegateOrSelf(C_00860924* self, void* /*edx*/, uint32_t* a2, int* a3)
{
    if (HudCheckFlagAgainstGlobal(a2, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBAFC))))
    {
        *a3 = (int)self;
        return self != nullptr;
    }

    C_00860178_ResolveDelegate(reinterpret_cast<C_00860178*>(self), nullptr, a2, reinterpret_cast<uint32_t*>(a3));
    return *a3 != 0;
}

HOOK(0x006EFC30, C_00860924_ResolveDelegateOrSelf, "auto");
