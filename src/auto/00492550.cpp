#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_DefaultLabelHeight, 0x00833C54);
CSF_GLOBAL(float, g_LabelHeightOffset, 0x00833C58);

// Sets the inner object's height/size value at +0x1D4 from one of three
// settings stored in the sub-object at +0x2C (+0xE4 / +0xE8 / +0xEC).
// Falls back to a per-mode default when the stored setting is not positive.
static void __fastcall SetInnerObjectMode(void* self, void* /*edx*/, int mode)
{
    float value = g_fZero;

    switch (mode)
    {
    case 0:
        value = CSF_AT(float, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x2C)), 0xE4);
        break;
    case 1:
        value = CSF_AT(float, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x2C)), 0xE8);
        break;
    case 2:
        value = CSF_AT(float, reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x2C)), 0xEC);
        break;
    default:
        break;
    }

    // Positive (or unordered/NaN) setting: use it directly.
    if (!(value <= g_fZero))
    {
        CSF_AT(float, self, 0x1D4) = value;
        return;
    }

    if (mode == 0)
    {
        CSF_AT(float, self, 0x1D4) = g_DefaultLabelHeight;
        return;
    }
    if (mode == 1)
    {
        CSF_AT(float, self, 0x1D4) = g_MaxPercent;
        return;
    }
    if (mode == 2)
    {
        CSF_AT(float, self, 0x1D4) = g_LabelHeightOffset;
        return;
    }

    CSF_AT(float, self, 0x1D4) = value;
}

HOOK(0x492550, SetInnerObjectMode, "auto");
