// Auto-generated reimplementation for 0x006B3780 (C_00860288 method).
// Packs four float channels (scaled by g_ColorChannelScale and truncated to bytes)
// into one 32-bit value, fetches an entry from the object at field_0068 through
// its vtable, and stores the packed value into that entry via C_008670f0_SetEntry.

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* , int key, int value));

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

// Converts one normalized float channel (0..1) to an 8-bit value (0..255) by
// scaling and truncating toward zero.
static uint32_t ColorChannelToByte(float channel)
{
    return static_cast<uint32_t>(static_cast<int64_t>(channel * g_ColorChannelScale));
}

static int __fastcall C_00860288_PackColorIntoEntry(C_00860288* self, void* /*edx*/, float* a2)
{
    // Pack order mirrors the original: a2[3] first, then a2[0], a2[1], a2[2].
    uint32_t packed = ColorChannelToByte(a2[3]);
    packed = (packed << 8) | ColorChannelToByte(a2[0]);
    packed = (packed << 8) | ColorChannelToByte(a2[1]);
    packed = (packed << 8) | ColorChannelToByte(a2[2]);

    // Re-read the field after the conversions, as the original does.
    void* colorObject = reinterpret_cast<void*>(self->field_0068);

    // Virtual call through slot 0x1C (index 7) of the untyped object.
    void* (__fastcall* getEntry)(void*, void*, int, int) =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, colorObject, 0)[7]);
    C_008670f0* entry = reinterpret_cast<C_008670f0*>(
        getEntry(colorObject, nullptr, 6, static_cast<int>(packed)));

    return C_008670f0_SetEntry(entry, nullptr, 6, static_cast<int>(packed));
}

HOOK(0x006B3780, C_00860288_PackColorIntoEntry, "auto");
