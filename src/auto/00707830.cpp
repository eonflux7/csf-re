#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Stops the active animation on the interpolator's sub-object when one is running,
// then writes the stored value at +0x18 to outValue and clears the byte at outValue+4.
static int __fastcall AnimInterpolator_StopAndOutputTime(C_008633a0* self, void* /*edx*/, int outValue)
{
    if (self->field_0010 && self->field_0020)
    {
        // Virtual slot +0x24 (index 9) on the object at field_0008, called with arg 0.
        void* stopTarget = reinterpret_cast<void*>(self->field_0008);
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, stopTarget, 0)[0x24 / 4])(stopTarget, nullptr, 0);
        self->field_0020 = 0;
    }

    CSF_AT(uint32_t, outValue, 0) = self->field_0018;
    CSF_AT(uint8_t, outValue, 4) = 0;
    return outValue;
}

HOOK(0x707830, AnimInterpolator_StopAndOutputTime, "auto");
