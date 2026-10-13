#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudCheckFlagAgainstGlobal, 0x006B2800, (uint32_t* self, void* edx, uint32_t* other));
CSF_FUNC(bool, __fastcall, C_00860178_ResolveDelegate, 0x006B2840, (C_00860178* self, void* edx, uint32_t* identifier, uint32_t* delegateOut));

static int __fastcall C_008620c8_ResolveOrDelegate(C_008620c8* self, void* /*edx*/, uint32_t* identifier, int* delegateOut)
{
    if (HudCheckFlagAgainstGlobal(identifier, nullptr, reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BBEF0))))
    {
        *delegateOut = (int)self;
        return self != nullptr;
    }

    C_00860178_ResolveDelegate(reinterpret_cast<C_00860178*>(self), nullptr, identifier, reinterpret_cast<uint32_t*>(delegateOut));
    return *delegateOut != 0;
}

HOOK(0x00708090, C_008620c8_ResolveOrDelegate, "auto");
