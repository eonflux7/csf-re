#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CanConfigureEffect, 0x007319C0, (void* self, void* edx, int effect));

// Original: int __thiscall StartEffect(C_00867078* this, int a2), ret 4.
// If the object's flag at +0x18 is set, forward a2 to CanConfigureEffect.
// Otherwise return a2 directly when it is nonzero, or ask CanConfigureEffect with 0.
// A nonzero result gets its byte at +4 set to 1.
static int __fastcall C_00867078_StartEffect(C_00867078* self, void* /*edx*/, int a2)
{
    int result;

    if (self->field_0018)
    {
        result = CanConfigureEffect(self, nullptr, a2);
    }
    else
    {
        result = a2;
        if (a2)
            return result;
        result = CanConfigureEffect(self, nullptr, 0);
    }

    if (result)
        *reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result) + 4) = 1;

    return result;
}

HOOK(0x731A80, C_00867078_StartEffect, "auto");
